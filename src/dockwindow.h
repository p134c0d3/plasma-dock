#pragma once
#include <QQuickWindow>

class DockWindow : public QQuickWindow {
    Q_OBJECT
public:
    DockWindow();
    ~DockWindow();

private:
    void setupGeometry();
};