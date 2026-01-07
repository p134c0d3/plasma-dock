#include "monitor.h"
#include <QDebug>

Monitor::Monitor(QScreen *screen, QObject *parent)
    : QObject(parent)
    , m_screen(screen)
    , m_name(screen->name())
    , m_identifier(screenIdentifier(screen))
    , m_geometry(screen->geometry())
    , m_isPrimary(screen == QGuiApplication::primaryScreen())
{
    connect(screen, &QScreen::geometryChanged, this, &Monitor::geometryChanged);
    connect(screen, &QScreen::destroyed, this, [this]() {
        emit destroyed(this);
    });
}

Monitor::~Monitor() {
}

QString Monitor::name() const {
    return m_name;
}

QString Monitor::identifier() const {
    return m_identifier;
}

QRect Monitor::geometry() const {
    return m_geometry;
}

int Monitor::width() const {
    return m_geometry.width();
}

int Monitor::height() const {
    return m_geometry.height();
}

bool Monitor::isPrimary() const {
    return m_isPrimary;
}

QString Monitor::screenIdentifier(QScreen *screen) {
#ifdef Q_OS_LINUX
    QString envName = QString::fromLocal8Bit(qgetenv("WAYLAND_DISPLAY"));
    if (!envName.isEmpty()) {
        return QString("wayland:%1").arg(screen->name());
    }
#endif
    return QString("x11:%1").arg(screen->name());
}
