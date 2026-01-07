#include "dockwindow.h"
#include "config.h"
#include <QScreen>
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QQuickItem>

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
