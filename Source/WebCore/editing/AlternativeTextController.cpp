/*
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "AlternativeTextController.h"

#include "DocumentMarkerController.h"
#include "EditCommand.h"
#include "EditorClient.h"
#include "FloatQuad.h"
#include "Frame.h"
#include "FrameView.h"
#include "Page.h"
#include "SpellingCorrectionCommand.h"
#include "TextCheckerClient.h"
#include "TextCheckingHelper.h"
#include "TextIterator.h"
#include "VisibleSelection.h"
#include "htmlediting.h"
#include "markup.h"
#include "visible_units.h"

namespace WebCore {

using namespace std;
using namespace WTF;

class AutocorrectionAlternativeDetails : public AlternativeTextDetails {
public:
    static PassRefPtr<AutocorrectionAlternativeDetails> create(const String& replacementString)
    {
        return adoptRef(new AutocorrectionAlternativeDetails(replacementString));
    }
    
    const String& replacementString() const { return m_replacementString; }
private:
    AutocorrectionAlternativeDetails(const String& replacementString)
    : m_replacementString(replacementString)
    { }
    
    String m_replacementString;
};

#if USE(AUTOCORRECTION_PANEL)

static const Vector<DocumentMarker::MarkerType>& markerTypesForAutocorrection()
{
    DEFINE_STATIC_LOCAL(Vector<DocumentMarker::MarkerType>, markerTypesForAutoCorrection, ());
    if (markerTypesForAutoCorrection.isEmpty()) {
        markerTypesForAutoCorrection.append(DocumentMarker::Replacement);
        markerTypesForAutoCorrection.append(DocumentMarker::CorrectionIndicator);
        markerTypesForAutoCorrection.append(DocumentMarker::SpellCheckingExemption);
        markerTypesForAutoCorrection.append(DocumentMarker::Autocorrected);
    }
    return markerTypesForAutoCorrection;
}

static const Vector<DocumentMarker::MarkerType>& markerTypesForReplacement()
{
    DEFINE_STATIC_LOCAL(Vector<DocumentMarker::MarkerType>, markerTypesForReplacement, ());
    if (markerTypesForReplacement.isEmpty()) {
        markerTypesForReplacement.append(DocumentMarker::Replacement);
        markerTypesForReplacement.append(DocumentMarker::SpellCheckingExemption);
    }
    return markerTypesForReplacement;
}

static bool markersHaveIdenticalDescription(const Vector<DocumentMarker*>& markers)
{
    if (markers.isEmpty())
        return true;

    const String& description = markers[0]->description();
    for (size_t i = 1; i < markers.size(); ++i) {
        if (description != markers[i]->description())
            return false;
    }
    return true;
}

AlternativeTextController::AlternativeTextController(Frame* frame)
    : m_frame(frame)
    , m_timer(this, &AlternativeTextController::timerFired)
{
}

AlternativeTextController::~AlternativeTextController()
{
    dismiss(ReasonForDismissingAlternativeTextIgnored);
}

void AlternativeTextController::startAlternativeTextUITimer(AlternativeTextType type)
{
    const double correctionPanelTimerInterval = 0.3;
    if (!isAutomaticSpellingCorrectionEnabled())
        return;

    // If type is PanelTypeReversion, then the new range has been set. So we shouldn't clear it.
    if (type == AlternativeTextTypeCorrection)
        m_alternativeTextInfo.rangeWithAlternative.clear();
    m_alternativeTextInfo.type = type;
    m_timer.startOneShot(correctionPanelTimerInterval);
}

void AlternativeTextController::stopAlternativeTextUITimer()
{
    m_timer.stop();
    m_alternativeTextInfo.rangeWithAlternative.clear();
}

void AlternativeTextController::stopPendingCorrection(const VisibleSelection& oldSelection)
{
    // Make sure there's no pending autocorrection before we call markMisspellingsAndBadGrammar() below.
    VisibleSelection currentSelection(m_frame->selection()->selection());
    if (currentSelection == oldSelection)
        return;

    stopAlternativeTextUITimer();
    dismiss(ReasonForDismissingAlternativeTextIgnored);
}

void AlternativeTextController::applyPendingCorrection(const VisibleSelection& selectionAfterTyping)
{
    // Apply pending autocorrection before next round of spell checking.
    bool doApplyCorrection = true;
    VisiblePosition startOfSelection = selectionAfterTyping.visibleStart();
    VisibleSelection currentWord = VisibleSelection(startOfWord(startOfSelection, LeftWordIfOnBoundary), endOfWord(startOfSelection, RightWordIfOnBoundary));
    if (currentWord.visibleEnd() == startOfSelection) {
        String wordText = plainText(currentWord.toNormalizedRange().get());
        if (wordText.length() > 0 && isAmbiguousBoundaryCharacter(wordText[wordText.length() - 1]))
            doApplyCorrection = false;
    }
    if (doApplyCorrection)
        handleAlternativeTextUIResult(dismissSoon(ReasonForDismissingAlternativeTextAccepted)); 
    else
        m_alternativeTextInfo.rangeWithAlternative.clear();
}

bool AlternativeTextController::hasPendingCorrection() const
{
    return m_alternativeTextInfo.rangeWithAlternative;
}

bool AlternativeTextController::isSpellingMarkerAllowed(PassRefPtr<Range> misspellingRange) const
{
    return !m_frame->document()->markers()->hasMarkers(misspellingRange.get(), DocumentMarker::SpellCheckingExemption);
}

void AlternativeTextController::show(PassRefPtr<Range> rangeToReplace, const String& replacement)
{
    FloatRect boundingBox = rootViewRectForRange(rangeToReplace.get());
    if (boundingBox.isEmpty())
        return;
    m_alternativeTextInfo.originalText = plainText(rangeToReplace.get());
    m_alternativeTextInfo.rangeWithAlternative = rangeToReplace;
    m_alternativeTextInfo.details = AutocorrectionAlternativeDetails::create(replacement);
    m_alternativeTextInfo.isActive = true;
    client()->showCorrectionPanel(m_alternativeTextInfo.type, boundingBox, m_alternativeTextInfo.originalText, replacement, Vector<String>());
}

void AlternativeTextController::handleCancelOperation()
{
    if (!m_alternativeTextInfo.isActive)
        return;
    m_alternativeTextInfo.isActive = false;
    dismiss(ReasonForDismissingAlternativeTextCancelled);
}

void AlternativeTextController::dismiss(ReasonForDismissingAlternativeText reasonForDismissing)
{
    if (!m_alternativeTextInfo.isActive)
        return;
    m_alternativeTextInfo.isActive = false;
    m_isDismissedByEditing = true;
    if (client())
        client()->dismissCorrectionPanel(reasonForDismissing);
}

String AlternativeTextController::dismissSoon(ReasonForDismissingAlternativeText reasonForDismissing)
{
    if (!m_alternativeTextInfo.isActive)
        return String();
    m_alternativeTextInfo.isActive = false;
    m_isDismissedByEditing = true;
    if (!client())
        return String();
    return client()->dismissCorrectionPanelSoon(reasonForDismissing);
}

void AlternativeTextController::applyAlternativeText(const String& alternative, const Vector<DocumentMarker::MarkerType>& markerTypesToAdd)
{
    if (!m_alternativeTextInfo.rangeWithAlternative)
        return;

    ExceptionCode ec = 0;
    RefPtr<Range> paragraphRangeContainingCorrection = m_alternativeTextInfo.rangeWithAlternative->cloneRange(ec);
    if (ec)
        return;

    setStart(paragraphRangeContainingCorrection.get(), startOfParagraph(m_alternativeTextInfo.rangeWithAlternative->startPosition()));
    setEnd(paragraphRangeContainingCorrection.get(), endOfParagraph(m_alternativeTextInfo.rangeWithAlternative->endPosition()));

    // After we replace the word at range rangeWithAlternative, we need to add markers to that range.
    // However, once the replacement took place, the value of rangeWithAlternative is not valid anymore.
    // So before we carry out the replacement, we need to store the start position of rangeWithAlternative
    // relative to the start position of the containing paragraph. We use correctionStartOffsetInParagraph
    // to store this value. In order to obtain this offset, we need to first create a range
    // which spans from the start of paragraph to the start position of rangeWithAlternative.
    RefPtr<Range> correctionStartOffsetInParagraphAsRange = Range::create(paragraphRangeContainingCorrection->startContainer(ec)->document(), paragraphRangeContainingCorrection->startPosition(), paragraphRangeContainingCorrection->startPosition());
    if (ec)
        return;

    Position startPositionOfrangeWithAlternative = m_alternativeTextInfo.rangeWithAlternative->startPosition();
    correctionStartOffsetInParagraphAsRange->setEnd(startPositionOfrangeWithAlternative.containerNode(), startPositionOfrangeWithAlternative.computeOffsetInContainerNode(), ec);
    if (ec)
        return;

    // Take note of the location of autocorrection so that we can add marker after the replacement took place.
    int correctionStartOffsetInParagraph = TextIterator::rangeLength(correctionStartOffsetInParagraphAsRange.get());

    // Clone the range, since the caller of this method may want to keep the original range around.
    RefPtr<Range> rangeWithAlternative = m_alternativeTextInfo.rangeWithAlternative->cloneRange(ec);
    applyCommand(SpellingCorrectionCommand::create(rangeWithAlternative, alternative));
    setEnd(paragraphRangeContainingCorrection.get(), m_frame->selection()->selection().start());
    RefPtr<Range> replacementRange = TextIterator::subrange(paragraphRangeContainingCorrection.get(), correctionStartOffsetInParagraph, alternative.length());
    String newText = plainText(replacementRange.get());

    // Check to see if replacement succeeded.
    if (newText != alternative)
        return;

    DocumentMarkerController* markers = replacementRange->startContainer()->document()->markers();
    size_t size = markerTypesToAdd.size();
    for (size_t i = 0; i < size; ++i) {
        DocumentMarker::MarkerType markerType = markerTypesToAdd[i];
        String description;
        if (m_alternativeTextInfo.type != AlternativeTextTypeReversion && (markerType == DocumentMarker::Replacement || markerType == DocumentMarker::Autocorrected))
            description = m_alternativeTextInfo.originalText;
        markers->addMarker(replacementRange.get(), markerType, description);
    }
}

bool AlternativeTextController::applyAutocorrectionBeforeTypingIfAppropriate()
{
    if (!m_alternativeTextInfo.rangeWithAlternative || !m_alternativeTextInfo.isActive)
        return false;

    if (m_alternativeTextInfo.type != AlternativeTextTypeCorrection)
        return false;

    Position caretPosition = m_frame->selection()->selection().start();

    if (m_alternativeTextInfo.rangeWithAlternative->endPosition() == caretPosition) {
        handleAlternativeTextUIResult(dismissSoon(ReasonForDismissingAlternativeTextAccepted));
        return true;
    } 
    
    // Pending correction should always be where caret is. But in case this is not always true, we still want to dismiss the panel without accepting the correction.
    ASSERT(m_alternativeTextInfo.rangeWithAlternative->endPosition() == caretPosition);
    dismiss(ReasonForDismissingAlternativeTextIgnored);
    return false;
}

void AlternativeTextController::respondToUnappliedSpellCorrection(const VisibleSelection& selectionOfCorrected, const String& corrected, const String& correction)
{
    client()->recordAutocorrectionResponse(EditorClient::AutocorrectionReverted, corrected, correction);
    m_frame->document()->updateLayout();
    m_frame->selection()->setSelection(selectionOfCorrected, FrameSelection::CloseTyping | FrameSelection::ClearTypingStyle | FrameSelection::SpellCorrectionTriggered);
    RefPtr<Range> range = Range::create(m_frame->document(), m_frame->selection()->selection().start(), m_frame->selection()->selection().end());

    DocumentMarkerController* markers = m_frame->document()->markers();
    markers->removeMarkers(range.get(), DocumentMarker::Spelling | DocumentMarker::Autocorrected, DocumentMarkerController::RemovePartiallyOverlappingMarker);
    markers->addMarker(range.get(), DocumentMarker::Replacement);
    markers->addMarker(range.get(), DocumentMarker::SpellCheckingExemption);
}

void AlternativeTextController::timerFired(Timer<AlternativeTextController>*)
{
    m_isDismissedByEditing = false;
    switch (m_alternativeTextInfo.type) {
    case AlternativeTextTypeCorrection: {
        VisibleSelection selection(m_frame->selection()->selection());
        VisiblePosition start(selection.start(), selection.affinity());
        VisiblePosition p = startOfWord(start, LeftWordIfOnBoundary);
        VisibleSelection adjacentWords = VisibleSelection(p, start);
        m_frame->editor()->markAllMisspellingsAndBadGrammarInRanges(TextCheckingTypeSpelling | TextCheckingTypeShowCorrectionPanel, adjacentWords.toNormalizedRange().get(), 0);
    }
        break;
    case AlternativeTextTypeReversion: {
        if (!m_alternativeTextInfo.rangeWithAlternative)
            break;
        m_alternativeTextInfo.isActive = true;
        m_alternativeTextInfo.originalText = plainText(m_alternativeTextInfo.rangeWithAlternative.get());
        FloatRect boundingBox = rootViewRectForRange(m_alternativeTextInfo.rangeWithAlternative.get());
        if (!boundingBox.isEmpty()) {
            const AutocorrectionAlternativeDetails* details = static_cast<const AutocorrectionAlternativeDetails*>(m_alternativeTextInfo.details.get());
            client()->showCorrectionPanel(m_alternativeTextInfo.type, boundingBox, m_alternativeTextInfo.originalText, details->replacementString(), Vector<String>());
        }
    }
        break;
    case AlternativeTextTypeSpellingSuggestions: {
        if (!m_alternativeTextInfo.rangeWithAlternative || plainText(m_alternativeTextInfo.rangeWithAlternative.get()) != m_alternativeTextInfo.originalText)
            break;
        String paragraphText = plainText(TextCheckingParagraph(m_alternativeTextInfo.rangeWithAlternative).paragraphRange().get());
        Vector<String> suggestions;
        textChecker()->getGuessesForWord(m_alternativeTextInfo.originalText, paragraphText, suggestions);
        if (suggestions.isEmpty()) {
            m_alternativeTextInfo.rangeWithAlternative.clear();
            break;
        }
        String topSuggestion = suggestions.first();
        suggestions.remove(0);
        m_alternativeTextInfo.isActive = true;
        FloatRect boundingBox = rootViewRectForRange(m_alternativeTextInfo.rangeWithAlternative.get());
        if (!boundingBox.isEmpty())
            client()->showCorrectionPanel(m_alternativeTextInfo.type, boundingBox, m_alternativeTextInfo.originalText, topSuggestion, suggestions);
    }
        break;
    }
}

void AlternativeTextController::handleAlternativeTextUIResult(const String& result)
{
    Range* replacedRange = m_alternativeTextInfo.rangeWithAlternative.get();
    if (!replacedRange || m_frame->document() != replacedRange->ownerDocument())
        return;

    String currentWord = plainText(m_alternativeTextInfo.rangeWithAlternative.get());
    // Check to see if the word we are about to correct has been changed between timer firing and callback being triggered.
    if (currentWord != m_alternativeTextInfo.originalText)
        return;

    m_alternativeTextInfo.isActive = false;

    switch (m_alternativeTextInfo.type) {
    case AlternativeTextTypeCorrection:
        if (result.length())
            applyAlternativeText(result, markerTypesForAutocorrection());
        else if (!m_isDismissedByEditing)
            replacedRange->startContainer()->document()->markers()->addMarker(replacedRange, DocumentMarker::RejectedCorrection, m_alternativeTextInfo.originalText);
        break;
    case AlternativeTextTypeReversion:
    case AlternativeTextTypeSpellingSuggestions:
        if (result.length())
            applyAlternativeText(result, markerTypesForReplacement());
        break;
    }

    m_alternativeTextInfo.rangeWithAlternative.clear();
}

bool AlternativeTextController::isAutomaticSpellingCorrectionEnabled()
{
    return client() && client()->isAutomaticSpellingCorrectionEnabled();
}

FloatRect AlternativeTextController::rootViewRectForRange(const Range* range) const
{
    FrameView* view = m_frame->view();
    if (!view)
        return FloatRect();
    Vector<FloatQuad> textQuads;
    range->textQuads(textQuads);
    FloatRect boundingRect;
    size_t size = textQuads.size();
    for (size_t i = 0; i < size; ++i)
        boundingRect.unite(textQuads[i].boundingBox());
    return view->contentsToRootView(IntRect(boundingRect));
}        

void AlternativeTextController::respondToChangedSelection(const VisibleSelection& oldSelection)
{
    VisibleSelection currentSelection(m_frame->selection()->selection());
    // When user moves caret to the end of autocorrected word and pauses, we show the panel
    // containing the original pre-correction word so that user can quickly revert the
    // undesired autocorrection. Here, we start correction panel timer once we confirm that
    // the new caret position is at the end of a word.
    if (!currentSelection.isCaret() || currentSelection == oldSelection)
        return;

    VisiblePosition selectionPosition = currentSelection.start();
    
    // Creating a Visible position triggers a layout and there is no
    // guarantee that the selection is still valid.
    if (selectionPosition.isNull())
        return;
    
    VisiblePosition endPositionOfWord = endOfWord(selectionPosition, LeftWordIfOnBoundary);
    if (selectionPosition != endPositionOfWord)
        return;

    Position position = endPositionOfWord.deepEquivalent();
    if (position.anchorType() != Position::PositionIsOffsetInAnchor)
        return;

    Node* node = position.containerNode();
    int endOffset = position.offsetInContainerNode();
    Vector<DocumentMarker*> markers = node->document()->markers()->markersFor(node);
    size_t markerCount = markers.size();
    for (size_t i = 0; i < markerCount; ++i) {
        const DocumentMarker* marker = markers[i];
        if (!shouldStartTimerFor(marker, endOffset))
            continue;
        RefPtr<Range> wordRange = Range::create(m_frame->document(), node, marker->startOffset(), node, marker->endOffset());
        String currentWord = plainText(wordRange.get());
        if (!currentWord.length())
            continue;

        m_alternativeTextInfo.rangeWithAlternative = wordRange;
        m_alternativeTextInfo.originalText = currentWord;
        if (marker->type() == DocumentMarker::Spelling)
            startAlternativeTextUITimer(AlternativeTextTypeSpellingSuggestions);
        else {
            m_alternativeTextInfo.details = AutocorrectionAlternativeDetails::create(marker->description());
            startAlternativeTextUITimer(AlternativeTextTypeReversion);
        }

        break;
    }
}

void AlternativeTextController::respondToAppliedEditing(CompositeEditCommand* command)
{
    if (command->isTopLevelCommand() && !command->shouldRetainAutocorrectionIndicator())
        m_frame->document()->markers()->removeMarkers(DocumentMarker::CorrectionIndicator);

    markPrecedingWhitespaceForDeletedAutocorrectionAfterCommand(command);
    m_originalStringForLastDeletedAutocorrection = String();
}

void AlternativeTextController::respondToUnappliedEditing(EditCommandComposition* command)
{
    if (!command->wasCreateLinkCommand())
        return;
    RefPtr<Range> range = Range::create(m_frame->document(), command->startingSelection().start(), command->startingSelection().end());
    if (!range)
        return;
    DocumentMarkerController* markers = m_frame->document()->markers();
    markers->addMarker(range.get(), DocumentMarker::Replacement);
    markers->addMarker(range.get(), DocumentMarker::SpellCheckingExemption);
}

EditorClient* AlternativeTextController::client()
{
    return m_frame->page() ? m_frame->page()->editorClient() : 0;
}

TextCheckerClient* AlternativeTextController::textChecker()
{
    if (EditorClient* owner = client())
        return owner->textChecker();
    return 0;
}

void AlternativeTextController::recordAutocorrectionResponseReversed(const String& replacedString, const String& replacementString)
{
    client()->recordAutocorrectionResponse(EditorClient::AutocorrectionReverted, replacedString, replacementString);
}

void AlternativeTextController::recordAutocorrectionResponseReversed(const String& replacedString, PassRefPtr<Range> replacementRange)
{
    recordAutocorrectionResponseReversed(replacedString, plainText(replacementRange.get()));
}

void AlternativeTextController::markReversed(PassRefPtr<Range> changedRange)
{
    changedRange->startContainer()->document()->markers()->removeMarkers(changedRange.get(), DocumentMarker::Autocorrected, DocumentMarkerController::RemovePartiallyOverlappingMarker);
    changedRange->startContainer()->document()->markers()->addMarker(changedRange.get(), DocumentMarker::SpellCheckingExemption);
}

void AlternativeTextController::markCorrection(PassRefPtr<Range> replacedRange, const String& replacedString)
{
    Vector<DocumentMarker::MarkerType> markerTypesToAdd = markerTypesForAutocorrection();
    DocumentMarkerController* markers = replacedRange->startContainer()->document()->markers();
    for (size_t i = 0; i < markerTypesToAdd.size(); ++i) {
        DocumentMarker::MarkerType markerType = markerTypesToAdd[i];
        if (markerType == DocumentMarker::Replacement || markerType == DocumentMarker::Autocorrected)
            markers->addMarker(replacedRange.get(), markerType, replacedString);
        else
            markers->addMarker(replacedRange.get(), markerType);
    }
}

void AlternativeTextController::recordSpellcheckerResponseForModifiedCorrection(Range* rangeOfCorrection, const String& corrected, const String& correction)
{
    if (!rangeOfCorrection)
        return;
    DocumentMarkerController* markers = rangeOfCorrection->startContainer()->document()->markers();
    Vector<DocumentMarker*> correctedOnceMarkers = markers->markersInRange(rangeOfCorrection, DocumentMarker::Autocorrected);
    if (correctedOnceMarkers.isEmpty())
        return;
    
    // Spelling corrected text has been edited. We need to determine whether user has reverted it to original text or
    // edited it to something else, and notify spellchecker accordingly.
    if (markersHaveIdenticalDescription(correctedOnceMarkers) && correctedOnceMarkers[0]->description() == corrected)
        client()->recordAutocorrectionResponse(EditorClient::AutocorrectionReverted, corrected, correction);
    else
        client()->recordAutocorrectionResponse(EditorClient::AutocorrectionEdited, corrected, correction);
    markers->removeMarkers(rangeOfCorrection, DocumentMarker::Autocorrected, DocumentMarkerController::RemovePartiallyOverlappingMarker);
}

void AlternativeTextController::deletedAutocorrectionAtPosition(const Position& position, const String& originalString)
{
    m_originalStringForLastDeletedAutocorrection = originalString;
    m_positionForLastDeletedAutocorrection = position;
}

void AlternativeTextController::markPrecedingWhitespaceForDeletedAutocorrectionAfterCommand(EditCommand* command)
{
    Position endOfSelection = command->endingSelection().end();
    if (endOfSelection != m_positionForLastDeletedAutocorrection)
        return;

    Position precedingCharacterPosition = endOfSelection.previous();
    if (endOfSelection == precedingCharacterPosition)
        return;

    RefPtr<Range> precedingCharacterRange = Range::create(m_frame->document(), precedingCharacterPosition, endOfSelection);
    String string = plainText(precedingCharacterRange.get());
    if (string.isEmpty() || !isWhitespace(string[string.length() - 1]))
        return;

    // Mark this whitespace to indicate we have deleted an autocorrection following this
    // whitespace. So if the user types the same original word again at this position, we
    // won't autocorrect it again.
    m_frame->document()->markers()->addMarker(precedingCharacterRange.get(), DocumentMarker::DeletedAutocorrection, m_originalStringForLastDeletedAutocorrection);
}

bool AlternativeTextController::processMarkersOnTextToBeReplacedByResult(const TextCheckingResult* result, Range* rangeWithAlternative, const String& stringToBeReplaced)
{
    DocumentMarkerController* markerController = m_frame->document()->markers();
    if (markerController->hasMarkers(rangeWithAlternative, DocumentMarker::Replacement)) {
        if (result->type == TextCheckingTypeCorrection)
            recordSpellcheckerResponseForModifiedCorrection(rangeWithAlternative, stringToBeReplaced, result->replacement);
        return false;
    }

    if (markerController->hasMarkers(rangeWithAlternative, DocumentMarker::RejectedCorrection))
        return false;

    Position beginningOfRange = rangeWithAlternative->startPosition();
    Position precedingCharacterPosition = beginningOfRange.previous();
    RefPtr<Range> precedingCharacterRange = Range::create(m_frame->document(), precedingCharacterPosition, beginningOfRange);

    Vector<DocumentMarker*> markers = markerController->markersInRange(precedingCharacterRange.get(), DocumentMarker::DeletedAutocorrection);

    for (size_t i = 0; i < markers.size(); ++i) {
        if (markers[i]->description() == stringToBeReplaced)
            return false;
    }

    return true;
}
    
#endif

} // namespace WebCore
