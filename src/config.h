#pragma once
#include <QObject>
#include <QColor>
#include <QString>
#include <QStringList>

class Config : public QObject {
    Q_OBJECT
public:
    enum MonitorMode {
        AllMonitors,
        PrimaryMonitor,
        SpecificMonitors
    };
    Q_ENUM(MonitorMode)

    enum VisibilityMode {
        AlwaysVisible,
        AutoHide,
        IntelliHide
    };
    Q_ENUM(VisibilityMode)

    explicit Config(QObject *parent = nullptr);
    ~Config();

    void load(const QString &configPath = QString());
    void reload();

    MonitorMode monitorMode() const;
    QStringList specificMonitors() const;
    VisibilityMode visibilityMode() const;
    int autoHideDelayMs() const;

    QColor backgroundColor() const;
    double opacity() const;
    bool blurEnabled() const;
    bool borderEnabled() const;
    QColor borderColor() const;
    int borderThickness() const;
    int cornerRadius() const;

    int dockHeight() const;
    int dockMargin() const;
    QString iconTheme() const;
    bool showTooltips() const;
    bool runningIndicatorEnabled() const;

    QString trashCommand() const;

    static QString defaultConfigPath();
    static QString defaultPinnedAppsPath();

signals:
    void configChanged();

private:
    void applyDefaults();

    MonitorMode m_monitorMode;
    QStringList m_specificMonitors;
    VisibilityMode m_visibilityMode;
    int m_autoHideDelayMs;

    QColor m_backgroundColor;
    double m_opacity;
    bool m_blurEnabled;
    bool m_borderEnabled;
    QColor m_borderColor;
    int m_borderThickness;
    int m_cornerRadius;

    int m_dockHeight;
    int m_dockMargin;
    QString m_iconTheme;
    bool m_showTooltips;
    bool m_runningIndicatorEnabled;

    QString m_trashCommand;

    QString m_configPath;
};
