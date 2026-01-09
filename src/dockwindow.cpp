#include "dockwindow.h"
#include "config.h"
#include <QScreen>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>
#include <QDebug>
#include <LayerShellQt/Window>
#include <KWindowSystem>
#include <KWindowEffects>

DockWindow::DockWindow(QScreen *screen, Config *config, QQuickWindow *parent)
    : QQuickWindow(parent)
    , m_screen(screen)
    , m_config(config)
    , m_isHidden(false)
    , m_rootItem(nullptr)
{
    setScreen(screen);

    m_monitorIdentifier = QString("%1:%2").arg(screen->name()).arg(screen->geometry().x());

    connect(screen, &QScreen::geometryChanged, this, &DockWindow::updateGeometry);
    connect(m_config, &Config::configChanged, this, &DockWindow::onConfigChanged);

    setFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setColor(Qt::transparent);

    applyConfig();
    updateGeometry();
    loadQmlContent();
    
    // Configure as dock/panel based on session type
    QString sessionType = qgetenv("XDG_SESSION_TYPE");
    if (sessionType == "wayland") {
        setupWayland();
    } else if (sessionType == "x11" || sessionType.isEmpty()) {
        // Fallback to X11 for non-Wayland sessions
        setupX11Fallback();
    }
    
    // Apply blur effect after window setup
    applyBlurEffect();
    
    show();
}

DockWindow::~DockWindow() {
}

QString DockWindow::monitorIdentifier() const {
    return m_monitorIdentifier;
}

void DockWindow::updateGeometry() {
    if (!m_screen) return;

    QRect geo = m_screen->geometry();

    int dockWidth = qMin(geo.width() - 2 * m_config->dockMargin(), 1600);
    int dockHeight = m_config->dockHeight();

    setWidth(dockWidth);
    setHeight(dockHeight);

    int x = geo.center().x() - dockWidth / 2;
    int y = geo.bottom() - dockHeight - m_config->dockMargin();

    setX(x);
    setY(y);
}

void DockWindow::updateVisibility() {
    if (!m_config) return;

    Config::VisibilityMode mode = m_config->visibilityMode();

    switch (mode) {
    case Config::AlwaysVisible:
        show();
        break;
    case Config::AutoHide:
    case Config::IntelliHide:
        show();
        break;
    }
}

void DockWindow::onScreenChanged(QScreen *screen) {
    setScreen(screen);
    m_screen = screen;
    updateGeometry();
}

void DockWindow::onConfigChanged() {
    applyConfig();
    updateGeometry();
    updateVisibility();
}

void DockWindow::setupWayland() {
    // Get the LayerShellQt window interface for this QWindow
    auto layerWindow = LayerShellQt::Window::get(this);
    if (!layerWindow) {
        qWarning() << "Failed to get LayerShellQt window interface";
        return;
    }

    // Configure as a top-layer panel/dock
    layerWindow->setLayer(LayerShellQt::Window::LayerTop);

    // Anchor to bottom of screen (and full width)
    auto anchors = static_cast<LayerShellQt::Window::Anchors>(
        LayerShellQt::Window::AnchorBottom | 
        LayerShellQt::Window::AnchorLeft | 
        LayerShellQt::Window::AnchorRight
    );
    layerWindow->setAnchors(anchors);

    // Reserve screen space so other windows don't overlap the dock
    layerWindow->setExclusiveZone(height());

    // Allow keyboard input when focused
    layerWindow->setKeyboardInteractivity(LayerShellQt::Window::KeyboardInteractivityOnDemand);

    // Set a scope identifier for the compositor
    layerWindow->setScope("plasma-dock");

    qDebug() << "Wayland layer-shell dock configured for monitor:" << m_monitorIdentifier;
}

void DockWindow::setupX11Fallback() {
    // For X11 sessions, the window manager will handle the frameless window hint
    // The Qt flags we set (Qt::Tool, Qt::FramelessWindowHint) are sufficient for X11
    // to recognize this as a dock-like window
    qDebug() << "X11 fallback dock configured for monitor:" << m_monitorIdentifier;
}

void DockWindow::applyBlurEffect() {
    // Apply blur effect using KWindowEffects (compositor-level blur)
    if (!m_config->blurEnabled()) {
        return;
    }

    // Enable blur behind the window using KWindowEffects
    // This creates a frosted glass effect on supported compositors (KWin)
    // Works on both Wayland and X11 via KWindowSystem abstraction
    KWindowEffects::enableBlurBehind(this, true, QRegion());
    qDebug() << "Blur effect enabled for dock on monitor:" << m_monitorIdentifier;
}

void DockWindow::applyConfig() {
    setOpacity(m_config->opacity());

    if (m_rootItem && m_rootItem->property("config").isValid()) {
        QVariant configVariant = QVariant::fromValue(m_config);
        m_rootItem->setProperty("config", configVariant);
    }

    if (m_rootItem && m_rootItem->property("dockHeight").isValid()) {
        m_rootItem->setProperty("dockHeight", m_config->dockHeight());
    }
}

void DockWindow::loadQmlContent() {
    QQmlEngine *engine = new QQmlEngine(this);
    QQmlComponent component(engine, QUrl("qrc:/qml/main.qml"));

    if (component.status() == QQmlComponent::Ready) {
        m_rootItem = qobject_cast<QQuickItem*>(component.create());
        if (m_rootItem) {
            m_rootItem->setParentItem(contentItem());

            if (m_rootItem->property("config").isValid()) {
                QVariant configVariant = QVariant::fromValue(m_config);
                m_rootItem->setProperty("config", configVariant);
            }

            if (m_rootItem->property("dockHeight").isValid()) {
                m_rootItem->setProperty("dockHeight", m_config->dockHeight());
            }
        }
    } else {
        qWarning() << "QML component error:" << component.errorString();
    }
}
