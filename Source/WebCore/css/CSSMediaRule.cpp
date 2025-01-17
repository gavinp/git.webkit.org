/**
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2005, 2006 Apple Computer, Inc.
 * Copyright (C) 2006 Samuel Weinig (sam@webkit.org)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CSSMediaRule.h"

#include "CSSParser.h"
#include "CSSRuleList.h"
#include "ExceptionCode.h"
#include "StyleRule.h"
#include <wtf/text/StringBuilder.h>

namespace WebCore {

CSSMediaRule::CSSMediaRule(StyleRuleMedia* mediaRule, CSSStyleSheet* parent)
    : CSSRule(parent, CSSRule::MEDIA_RULE)
    , m_mediaRule(mediaRule)
    , m_childRuleCSSOMWrappers(mediaRule->childRules().size())
{
}

CSSMediaRule::~CSSMediaRule()
{
    ASSERT(m_childRuleCSSOMWrappers.size() == m_mediaRule->childRules().size());

    for (unsigned i = 0; i < m_childRuleCSSOMWrappers.size(); ++i) {
        if (m_childRuleCSSOMWrappers[i])
            m_childRuleCSSOMWrappers[i]->setParentRule(0);
    }
}

unsigned CSSMediaRule::insertRule(const String& ruleString, unsigned index, ExceptionCode& ec)
{
    ASSERT(m_childRuleCSSOMWrappers.size() == m_mediaRule->childRules().size());

    if (index > m_mediaRule->childRules().size()) {
        // INDEX_SIZE_ERR: Raised if the specified index is not a valid insertion point.
        ec = INDEX_SIZE_ERR;
        return 0;
    }

    CSSParser p(cssParserMode());
    RefPtr<StyleRuleBase> newRule = p.parseRule(parentStyleSheet(), ruleString);
    if (!newRule) {
        // SYNTAX_ERR: Raised if the specified rule has a syntax error and is unparsable.
        ec = SYNTAX_ERR;
        return 0;
    }

    if (newRule->isImportRule()) {
        // FIXME: an HIERARCHY_REQUEST_ERR should also be thrown for a @charset or a nested
        // @media rule.  They are currently not getting parsed, resulting in a SYNTAX_ERR
        // to get raised above.

        // HIERARCHY_REQUEST_ERR: Raised if the rule cannot be inserted at the specified
        // index, e.g., if an @import rule is inserted after a standard rule set or other
        // at-rule.
        ec = HIERARCHY_REQUEST_ERR;
        return 0;
    }
    m_mediaRule->wrapperInsertRule(index, newRule);

    m_childRuleCSSOMWrappers.insert(index, RefPtr<CSSRule>());

    if (CSSStyleSheet* styleSheet = parentStyleSheet())
        styleSheet->styleSheetChanged();

    return index;
}

void CSSMediaRule::deleteRule(unsigned index, ExceptionCode& ec)
{
    ASSERT(m_childRuleCSSOMWrappers.size() == m_mediaRule->childRules().size());

    if (index >= m_mediaRule->childRules().size()) {
        // INDEX_SIZE_ERR: Raised if the specified index does not correspond to a
        // rule in the media rule list.
        ec = INDEX_SIZE_ERR;
        return;
    }
    m_mediaRule->wrapperRemoveRule(index);

    if (m_childRuleCSSOMWrappers[index])
        m_childRuleCSSOMWrappers[index]->setParentRule(0);
    m_childRuleCSSOMWrappers.remove(index);

    if (CSSStyleSheet* styleSheet = parentStyleSheet())
        styleSheet->styleSheetChanged();
}

String CSSMediaRule::cssText() const
{
    StringBuilder result;
    result.append("@media ");
    if (m_mediaRule->mediaQueries()) {
        result.append(m_mediaRule->mediaQueries()->mediaText());
        result.append(' ');
    }
    result.append("{ \n");
    
    unsigned size = length();
    for (unsigned i = 0; i < size; ++i) {
        result.append("  ");
        result.append(item(i)->cssText());
        result.append('\n');
    }

    result.append('}');
    return result.toString();
}

MediaList* CSSMediaRule::media() const 
{ 
    return m_mediaRule->mediaQueries() ? m_mediaRule->mediaQueries()->ensureMediaList(parentStyleSheet()) : 0; 
}

unsigned CSSMediaRule::length() const
{ 
    return m_mediaRule->childRules().size(); 
}

CSSRule* CSSMediaRule::item(unsigned index) const
{ 
    if (index >= length())
        return 0;
    ASSERT(m_childRuleCSSOMWrappers.size() == m_mediaRule->childRules().size());
    RefPtr<CSSRule>& rule = m_childRuleCSSOMWrappers[index];
    if (!rule)
        rule = m_mediaRule->childRules()[index]->createCSSOMWrapper(const_cast<CSSMediaRule*>(this));
    return rule.get();
}

CSSRuleList* CSSMediaRule::cssRules() const
{
    if (!m_ruleListCSSOMWrapper)
        m_ruleListCSSOMWrapper = adoptPtr(new LiveCSSRuleList<CSSMediaRule>(const_cast<CSSMediaRule*>(this)));
    return m_ruleListCSSOMWrapper.get();
}

} // namespace WebCore
