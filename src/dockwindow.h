#pragma once
#include <QQuickWindow>
#include <QScreen>
#include <QString>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickItem>

class Config;
class DockWindow : public QQuickWindow {
    Q_OBJECT
    Q_PROPERTY(QString monitorIdentifier READ monitorIdentifier CONSTANT)

public:
    DockWindow(QScreen *screen, Config *config, QQuickWindow *parent = nullptr);
    ~DockWindow();

    QString monitorIdentifier() const;

    void updateGeometry();
    void updateVisibility();

public slots:
    void onScreenChanged(QScreen *screen);
    void onConfigChanged();

signals:
    void dockHeightChanged(int height);
    void requestShow();
    void requestHide();

private:
    void setupWayland();
    void applyConfig();
    void loadQmlContent();

    QScreen *m_screen;
    Config *m_config;
    QString m_monitorIdentifier;
    bool m_isHidden;
    QQuickItem *m_rootItem;
};
