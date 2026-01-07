#include "dockwindow.h"
#include <QScreen>
#include <QGuiApplication>

DockWindow::DockWindow() {
    setupGeometry();
}

DockWindow::~DockWindow() {
}

void DockWindow::setupGeometry() {
    QScreen *screen = QGuiApplication::primaryScreen();
    setWidth(400);
    setHeight(48);
    setX(screen->geometry().center().x() - width() / 2);
    setY(screen->geometry().bottom() - height() - 10);
}