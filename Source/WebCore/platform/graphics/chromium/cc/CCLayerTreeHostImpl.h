/*
 * Copyright (C) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef CCLayerTreeHostImpl_h
#define CCLayerTreeHostImpl_h

#include "LayerRendererChromium.h"
#include "cc/CCAnimationEvents.h"
#include "cc/CCInputHandler.h"
#include "cc/CCLayerSorter.h"
#include "cc/CCLayerTreeHost.h"
#include "cc/CCLayerTreeHostCommon.h"
#include "cc/CCRenderPass.h"
#include <wtf/PassOwnPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {

class CCActiveGestureAnimation;
class CCCompletionEvent;
class CCPageScaleAnimation;
class CCLayerImpl;
class CCLayerTreeHostImplTimeSourceAdapter;
class LayerRendererChromium;
class TextureAllocator;
struct LayerRendererCapabilities;
class TransformationMatrix;

// CCLayerTreeHost->CCProxy callback interface.
class CCLayerTreeHostImplClient {
public:
    virtual void didLoseContextOnImplThread() = 0;
    virtual void onSwapBuffersCompleteOnImplThread() = 0;
    virtual void setNeedsRedrawOnImplThread() = 0;
    virtual void setNeedsCommitOnImplThread() = 0;
    virtual void postAnimationEventsToMainThreadOnImplThread(PassOwnPtr<CCAnimationEventsVector>, double wallClockTime) = 0;
};

// CCLayerTreeHostImpl owns the CCLayerImpl tree as well as associated rendering state
class CCLayerTreeHostImpl : public CCInputHandlerClient, LayerRendererChromiumClient {
    WTF_MAKE_NONCOPYABLE(CCLayerTreeHostImpl);
    typedef Vector<CCLayerImpl*> CCLayerList;

public:
    static PassOwnPtr<CCLayerTreeHostImpl> create(const CCSettings&, CCLayerTreeHostImplClient*);
    virtual ~CCLayerTreeHostImpl();

    // CCInputHandlerClient implementation
    virtual CCInputHandlerClient::ScrollStatus scrollBegin(const IntPoint&, CCInputHandlerClient::ScrollInputType);
    virtual void scrollBy(const IntSize&);
    virtual void scrollEnd();
    virtual void pinchGestureBegin();
    virtual void pinchGestureUpdate(float, const IntPoint&);
    virtual void pinchGestureEnd();
    virtual void startPageScaleAnimation(const IntSize& targetPosition, bool anchorPoint, float pageScale, double startTime, double duration);
    virtual CCActiveGestureAnimation* activeGestureAnimation() { return m_activeGestureAnimation.get(); }
    // To clear an active animation, pass nullptr.
    virtual void setActiveGestureAnimation(PassOwnPtr<CCActiveGestureAnimation>);
    virtual void scheduleAnimation();

    struct FrameData {
        CCRenderPassList renderPasses;
        CCLayerList renderSurfaceLayerList;
    };

    // Virtual for testing.
    virtual void beginCommit();
    virtual void commitComplete();
    virtual void animate(double monotonicTime, double wallClockTime);
    // Returns false if we should try to avoid displaying the frame, because it has visible checkerboard during an animation.
    virtual bool prepareToDraw(FrameData&);
    virtual void drawLayers(const FrameData&);

    // LayerRendererChromiumClient implementation
    virtual const IntSize& viewportSize() const { return m_viewportSize; }
    virtual const CCSettings& settings() const { return m_settings; }
    virtual CCLayerImpl* rootLayer() { return m_rootLayerImpl.get(); }
    virtual const CCLayerImpl* rootLayer() const { return m_rootLayerImpl.get(); }
    virtual void didLoseContext();
    virtual void onSwapBuffersComplete();
    virtual void setFullRootLayerDamage();

    // Implementation

    // Returns false if there is no valid root layer and thus no content that can be drawn.
    bool canDraw();
    GraphicsContext3D* context();

    void finishAllRendering();
    int frameNumber() const { return m_frameNumber; }

    bool initializeLayerRenderer(PassRefPtr<GraphicsContext3D>);
    bool isContextLost();
    LayerRendererChromium* layerRenderer() { return m_layerRenderer.get(); }
    const LayerRendererCapabilities& layerRendererCapabilities() const;
    TextureAllocator* contentsTextureAllocator() const;

    bool swapBuffers();

    void readback(void* pixels, const IntRect&);

    void setRootLayer(PassOwnPtr<CCLayerImpl>);
    PassOwnPtr<CCLayerImpl> releaseRootLayer() { return m_rootLayerImpl.release(); }

    CCLayerImpl* scrollLayer() const { return m_scrollLayerImpl; }

    bool visible() const { return m_visible; }
    void setVisible(bool);

    int sourceFrameNumber() const { return m_sourceFrameNumber; }
    void setSourceFrameNumber(int frameNumber) { m_sourceFrameNumber = frameNumber; }

    void setViewportSize(const IntSize&);

    void setPageScaleFactorAndLimits(float pageScale, float minPageScale, float maxPageScale);
    float pageScale() const { return m_pageScale; }

    PassOwnPtr<CCScrollAndScaleSet> processScrollDeltas();

    void startPageScaleAnimation(const IntSize& tragetPosition, bool useAnchor, float scale, double durationSec);

    bool needsAnimateLayers() const { return m_needsAnimateLayers; }
    void setNeedsAnimateLayers() { m_needsAnimateLayers = true; }

    void setNeedsRedraw();

protected:
    CCLayerTreeHostImpl(const CCSettings&, CCLayerTreeHostImplClient*);

    void animatePageScale(double monotonicTime);
    void animateGestures(double monotonicTime);

    // Virtual for testing.
    virtual void animateLayers(double monotonicTime, double wallClockTime);

    CCLayerTreeHostImplClient* m_client;
    int m_sourceFrameNumber;
    int m_frameNumber;

private:
    void computeDoubleTapZoomDeltas(CCScrollAndScaleSet* scrollInfo);
    void computePinchZoomDeltas(CCScrollAndScaleSet* scrollInfo);
    void makeScrollAndScaleSet(CCScrollAndScaleSet* scrollInfo, const IntSize& scrollOffset, float pageScale);

    void setPageScaleDelta(float);
    void applyPageScaleDeltaToScrollLayer();
    void adjustScrollsForPageScaleChange(float);
    void updateMaxScrollPosition();
    void trackDamageForAllSurfaces(CCLayerImpl* rootDrawLayer, const CCLayerList& renderSurfaceLayerList);
    // Returns false if the frame should not be displayed.
    bool calculateRenderPasses(CCRenderPassList&, CCLayerList& renderSurfaceLayerList);
    void animateLayersRecursive(CCLayerImpl*, double monotonicTime, double wallClockTime, CCAnimationEventsVector*, bool& didAnimate, bool& needsAnimateLayers);
    IntSize contentSize() const;
    void sendDidLoseContextRecursive(CCLayerImpl*);
    void clearRenderSurfacesOnCCLayerImplRecursive(CCLayerImpl*);

    OwnPtr<LayerRendererChromium> m_layerRenderer;
    OwnPtr<CCLayerImpl> m_rootLayerImpl;
    CCLayerImpl* m_scrollLayerImpl;
    CCSettings m_settings;
    IntSize m_viewportSize;
    bool m_visible;

    float m_pageScale;
    float m_pageScaleDelta;
    float m_sentPageScaleDelta;
    float m_minPageScale, m_maxPageScale;

    // If this is true, it is necessary to traverse the layer tree ticking the animators.
    bool m_needsAnimateLayers;
    bool m_pinchGestureActive;
    IntPoint m_previousPinchAnchor;

    OwnPtr<CCPageScaleAnimation> m_pageScaleAnimation;
    OwnPtr<CCActiveGestureAnimation> m_activeGestureAnimation;

    // This is used for ticking animations slowly when hidden.
    OwnPtr<CCLayerTreeHostImplTimeSourceAdapter> m_timeSourceClientAdapter;

    CCLayerSorter m_layerSorter;

    FloatRect m_rootDamageRect;
};

};

#endif
