/*
    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if USE(UI_SIDE_COMPOSITING)
#include "LayerTreeHostProxy.h"

#include "LayerTreeHostMessages.h"
#include "UpdateInfo.h"
#include "WebCoreArgumentCoders.h"
#include "WebLayerTreeInfo.h"
#include "WebLayerTreeRenderer.h"
#include "WebPageProxy.h"
#include "WebProcessProxy.h"

namespace WebKit {

using namespace WebCore;

LayerTreeHostProxy::LayerTreeHostProxy(DrawingAreaProxy* drawingAreaProxy)
    : m_drawingAreaProxy(drawingAreaProxy)
    , m_renderer(adoptRef(new WebLayerTreeRenderer(this)))
{
}

LayerTreeHostProxy::~LayerTreeHostProxy()
{
    m_renderer->detach();
}

void LayerTreeHostProxy::paintToCurrentGLContext(const WebCore::TransformationMatrix& matrix, float opacity, const WebCore::FloatRect& rect)
{
    m_renderer->syncRemoteContent();
    m_renderer->paintToCurrentGLContext(matrix, opacity, rect);
}

void LayerTreeHostProxy::paintToGraphicsContext(BackingStore::PlatformGraphicsContext context)
{
    m_renderer->paintToGraphicsContext(context);
}

void LayerTreeHostProxy::updateViewport()
{
    m_drawingAreaProxy->updateViewport();
}

void LayerTreeHostProxy::dispatchUpdate(const Function<void()>& function)
{
    m_renderer->appendUpdate(function);
}

void LayerTreeHostProxy::createTileForLayer(int layerID, int tileID, const WebKit::UpdateInfo& updateInfo)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::createTile, m_renderer.get(), layerID, tileID, updateInfo.updateScaleFactor));
    updateTileForLayer(layerID, tileID, updateInfo);
}

void LayerTreeHostProxy::updateTileForLayer(int layerID, int tileID, const WebKit::UpdateInfo& updateInfo)
{
    ASSERT(updateInfo.updateRects.size() == 1);
    IntRect sourceRect = updateInfo.updateRects.first();
    IntRect targetRect = updateInfo.updateRectBounds;
    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::create(updateInfo.bitmapHandle);
    dispatchUpdate(bind(&WebLayerTreeRenderer::updateTile, m_renderer.get(), layerID, tileID, sourceRect, targetRect, bitmap));
}

void LayerTreeHostProxy::removeTileForLayer(int layerID, int tileID)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::removeTile, m_renderer.get(), layerID, tileID));
}

void LayerTreeHostProxy::deleteCompositingLayer(WebLayerID id)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::deleteLayer, m_renderer.get(), id));
    updateViewport();
}

void LayerTreeHostProxy::setRootCompositingLayer(WebLayerID id)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::setRootLayerID, m_renderer.get(), id));
    updateViewport();
}

void LayerTreeHostProxy::syncCompositingLayerState(const WebLayerInfo& info)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::syncLayerParameters, m_renderer.get(), info));
}

void LayerTreeHostProxy::didRenderFrame()
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::flushLayerChanges, m_renderer.get()));
    updateViewport();
}

void LayerTreeHostProxy::createDirectlyCompositedImage(int64_t key, const WebKit::ShareableBitmap::Handle& handle)
{
    RefPtr<ShareableBitmap> bitmap = ShareableBitmap::create(handle);
    dispatchUpdate(bind(&WebLayerTreeRenderer::createImage, m_renderer.get(), key, bitmap));
}

void LayerTreeHostProxy::destroyDirectlyCompositedImage(int64_t key)
{
    dispatchUpdate(bind(&WebLayerTreeRenderer::destroyImage, m_renderer.get(), key));
}

void LayerTreeHostProxy::setVisibleContentsRect(const IntRect& rect, float scale, const FloatPoint& trajectoryVector)
{
    m_renderer->setVisibleContentsRect(rect, scale);
    m_drawingAreaProxy->page()->process()->send(Messages::LayerTreeHost::SetVisibleContentsRect(rect, scale, trajectoryVector), m_drawingAreaProxy->page()->pageID());
}

void LayerTreeHostProxy::renderNextFrame()
{
    m_drawingAreaProxy->page()->process()->send(Messages::LayerTreeHost::RenderNextFrame(), m_drawingAreaProxy->page()->pageID());
}

void LayerTreeHostProxy::purgeBackingStores()
{
    m_drawingAreaProxy->page()->process()->send(Messages::LayerTreeHost::PurgeBackingStores(), m_drawingAreaProxy->page()->pageID());
}

}
#endif // USE(UI_SIDE_COMPOSITING)
