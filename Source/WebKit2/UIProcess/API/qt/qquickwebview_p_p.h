/*
 * Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this program; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#ifndef qquickwebview_p_p_h
#define qquickwebview_p_p_h

#include "DrawingAreaProxy.h"
#include "QtDialogRunner.h"
#include "QtPageClient.h"
#include "QtViewportInteractionEngine.h"
#include "QtWebPageLoadClient.h"
#include "QtWebPagePolicyClient.h"
#include "QtWebPageUIClient.h"
#include "QtWebUndoController.h"
#include "WebPageProxy.h"

#include "qquickwebview_p.h"
#include "qquickwebpage_p.h"

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <wtf/OwnPtr.h>
#include <wtf/RefPtr.h>

namespace WebKit {
class DownloadProxy;
class QtWebContext;
class WebPageProxy;
}
class QWebNavigationHistory;
class QWebViewportInfo;

QT_BEGIN_NAMESPACE
class QDeclarativeComponent;
QT_END_NAMESPACE

class QQuickWebViewPrivate {
    Q_DECLARE_PUBLIC(QQuickWebView)
    friend class QQuickWebViewExperimental;
    friend class QQuickWebPage;
    friend class QWebPreferencesPrivate;
    friend class QWebViewportInfo;

public:
    static QQuickWebViewPrivate* get(QQuickWebView* q) { return q->d_ptr.data(); }

    virtual ~QQuickWebViewPrivate();

    virtual void initialize(WKContextRef contextRef = 0, WKPageGroupRef pageGroupRef = 0);

    virtual void enableMouseEvents() { }
    virtual void disableMouseEvents() { }

    virtual QPointF pageItemPos();
    virtual void updateContentsSize(const QSizeF&) { }

    virtual void loadDidSucceed();
    virtual void onComponentComplete();
    virtual void loadDidCommit() { }
    virtual void didFinishFirstNonEmptyLayout() { }
    virtual void didChangeViewportProperties(const WebCore::ViewportArguments& args) { }
    void didChangeLoadingState(QWebLoadRequest* loadRequest);
    void didChangeBackForwardList();

    void setNeedsDisplay();

    virtual QtViewportInteractionEngine* viewportInteractionEngine() { return 0; }
    virtual void updateViewportSize() { }
    void updateTouchViewportSize();

    virtual void _q_suspend() { }
    virtual void _q_resume() { }
    virtual void _q_contentViewportChanged(const QPointF& trajectory) { };

    void _q_onVisibleChanged();
    void _q_onUrlChanged();
    void _q_onReceivedResponseFromDownload(QWebDownloadItem*);
    void _q_onIconChangedForPageURL(const QUrl& pageURL, const QUrl& iconURLString);

    void chooseFiles(WKOpenPanelResultListenerRef, const QStringList& selectedFileNames, QtWebPageUIClient::FileChooserType);
    quint64 exceededDatabaseQuota(const QString& databaseName, const QString& displayName, WKSecurityOriginRef securityOrigin, quint64 currentQuota, quint64 currentOriginUsage, quint64 currentDatabaseUsage, quint64 expectedUsage);
    void runJavaScriptAlert(const QString&);
    bool runJavaScriptConfirm(const QString&);
    QString runJavaScriptPrompt(const QString&, const QString& defaultValue, bool& ok);

    void handleAuthenticationRequiredRequest(const QString& hostname, const QString& realm, const QString& prefilledUsername, QString& username, QString& password);
    bool handleCertificateVerificationRequest(const QString& hostname);
    void handleProxyAuthenticationRequiredRequest(const QString& hostname, uint16_t port, const QString& prefilledUsername, QString& username, QString& password);

    void execDialogRunner(QtDialogRunner&);

    void setRenderToOffscreenBuffer(bool enable) { m_renderToOffscreenBuffer = enable; }
    void setTransparentBackground(bool);
    void setViewInAttachedProperties(QObject*);
    void setIcon(const QUrl&);

    bool navigatorQtObjectEnabled() const;
    bool renderToOffscreenBuffer() const { return m_renderToOffscreenBuffer; }
    bool transparentBackground() const;
    void setNavigatorQtObjectEnabled(bool);

    QPointF contentPos() const;
    void setContentPos(const QPointF&);

    QRect visibleContentsRect() const;

    void setDialogActive(bool active) { m_dialogActive = active; }

    // PageClient.
    WebCore::IntSize viewSize() const;
    void didReceiveMessageFromNavigatorQtObject(const String& message);
    virtual void pageDidRequestScroll(const QPoint& pos) { }
    virtual void didChangeContentsSize(const QSize& newSize) { }
    void processDidCrash();
    void didRelaunchProcess();
    PassOwnPtr<DrawingAreaProxy> createDrawingAreaProxy();
    void handleDownloadRequest(DownloadProxy*);

protected:
    QQuickWebViewPrivate(QQuickWebView* viewport);
    RefPtr<QtWebContext> context;
    RefPtr<WebKit::WebPageProxy> webPageProxy;

    QtPageClient pageClient;
    QtWebUndoController undoController;
    OwnPtr<QWebNavigationHistory> navigationHistory;
    OwnPtr<QWebPreferences> preferences;

    QScopedPointer<QtWebPageLoadClient> pageLoadClient;
    QScopedPointer<QtWebPagePolicyClient> pagePolicyClient;
    QScopedPointer<QtWebPageUIClient> pageUIClient;

    QScopedPointer<QQuickWebPage> pageView;
    QQuickWebView* q_ptr;

    QDeclarativeComponent* alertDialog;
    QDeclarativeComponent* confirmDialog;
    QDeclarativeComponent* promptDialog;
    QDeclarativeComponent* authenticationDialog;
    QDeclarativeComponent* certificateVerificationDialog;
    QDeclarativeComponent* itemSelector;
    QDeclarativeComponent* proxyAuthenticationDialog;
    QDeclarativeComponent* filePicker;
    QDeclarativeComponent* databaseQuotaDialog;

    WebCore::ViewportArguments viewportArguments;

    bool m_useDefaultContentItemSize;
    bool m_navigatorQtObjectEnabled;
    bool m_renderToOffscreenBuffer;
    bool m_loadStartedSignalSent;
    bool m_dialogActive;
    QUrl m_iconURL;
    QUrl m_deferedUrlToLoad;
};

class QQuickWebViewLegacyPrivate : public QQuickWebViewPrivate {
    Q_DECLARE_PUBLIC(QQuickWebView)
public:
    QQuickWebViewLegacyPrivate(QQuickWebView* viewport);
    virtual void initialize(WKContextRef contextRef = 0, WKPageGroupRef pageGroupRef = 0);

    virtual void updateViewportSize();
    virtual void enableMouseEvents();
    virtual void disableMouseEvents();
};

class QQuickWebViewFlickablePrivate : public QQuickWebViewPrivate {
    Q_DECLARE_PUBLIC(QQuickWebView)
public:
    QQuickWebViewFlickablePrivate(QQuickWebView* viewport);
    virtual ~QQuickWebViewFlickablePrivate();
    virtual void initialize(WKContextRef contextRef = 0, WKPageGroupRef pageGroupRef = 0);

    virtual QPointF pageItemPos();
    virtual void updateContentsSize(const QSizeF&);

    virtual void loadDidSucceed();
    virtual void onComponentComplete();
    virtual void loadDidCommit();
    virtual void didFinishFirstNonEmptyLayout();
    virtual void didChangeViewportProperties(const WebCore::ViewportArguments& args);
    virtual QtViewportInteractionEngine* viewportInteractionEngine() { return interactionEngine.data(); }
    virtual void updateViewportSize();

    virtual void _q_suspend();
    virtual void _q_resume();
    virtual void _q_contentViewportChanged(const QPointF& trajectory);

    virtual void pageDidRequestScroll(const QPoint& pos);
    virtual void didChangeContentsSize(const QSize& newSize);

    QtViewportInteractionEngine::Constraints computeViewportConstraints();

private:
    QScopedPointer<QtViewportInteractionEngine> interactionEngine;
    bool pageIsSuspended;
    bool loadSuccessDispatchIsPending;
};

#endif // qquickwebview_p_p_h
