#include "config.h"
#include "tomlparser.h"
#include <QDir>
#include <QFile>
#include <QStandardPaths>

Config::Config(QObject *parent)
    : QObject(parent)
    , m_monitorMode(AllMonitors)
    , m_visibilityMode(IntelliHide)
    , m_autoHideDelayMs(500)
    , m_backgroundColor(43, 43, 43)
    , m_opacity(0.8)
    , m_blurEnabled(true)
    , m_borderEnabled(true)
    , m_borderColor(60, 60, 60)
    , m_borderThickness(1)
    , m_cornerRadius(8)
    , m_dockHeight(48)
    , m_dockMargin(10)
    , m_iconTheme("breeze")
    , m_showTooltips(true)
    , m_runningIndicatorEnabled(true)
    , m_trashCommand("dolphin trash:/")
{
}

Config::~Config() {
}

void Config::load(const QString &configPath) {
    QString path = configPath;
    if (path.isEmpty()) {
        path = defaultConfigPath();
    }

    m_configPath = path;

    QFile file(path);
    if (!file.exists()) {
        applyDefaults();
        return;
    }

    TomlParser parser;
    if (!parser.load(path)) {
        applyDefaults();
        return;
    }

    QString monitorModeStr = parser.getString("display", "monitor_mode", "all");
    if (monitorModeStr == "primary") {
        m_monitorMode = PrimaryMonitor;
    } else if (monitorModeStr == "specific") {
        m_monitorMode = SpecificMonitors;
    } else {
        m_monitorMode = AllMonitors;
    }

    m_specificMonitors = parser.getStringList("display", "specific_monitors");

    QString visibilityStr = parser.getString("behavior", "visibility", "intelli-hide");
    if (visibilityStr == "always") {
        m_visibilityMode = AlwaysVisible;
    } else if (visibilityStr == "auto-hide") {
        m_visibilityMode = AutoHide;
    } else {
        m_visibilityMode = IntelliHide;
    }

    m_autoHideDelayMs = parser.getInt("behavior", "auto_hide_delay_ms", 500);

    m_backgroundColor = parser.getColor("appearance", "background_color", QColor(43, 43, 43));
    m_opacity = parser.getDouble("appearance", "opacity", 0.8);
    m_blurEnabled = parser.getBool("appearance", "blur_enabled", true);
    m_borderEnabled = parser.getBool("appearance", "border_enabled", true);
    m_borderColor = parser.getColor("appearance", "border_color", QColor(60, 60, 60));
    m_borderThickness = parser.getInt("appearance", "border_thickness", 1);
    m_cornerRadius = parser.getInt("appearance", "corner_radius", 8);

    m_dockHeight = parser.getInt("appearance", "dock_height", 48);
    m_dockMargin = parser.getInt("appearance", "dock_margin", 10);
    m_iconTheme = parser.getString("appearance", "icon_theme", "breeze");
    m_showTooltips = parser.getBool("appearance", "show_tooltips", true);
    m_runningIndicatorEnabled = parser.getBool("appearance", "running_indicator_enabled", true);

    m_trashCommand = parser.getString("trash", "command", "dolphin trash:/");

    emit configChanged();
}

void Config::reload() {
    load(m_configPath);
}

void Config::applyDefaults() {
    m_monitorMode = AllMonitors;
    m_specificMonitors.clear();
    m_visibilityMode = IntelliHide;
    m_autoHideDelayMs = 500;
    m_backgroundColor = QColor(43, 43, 43);
    m_opacity = 0.8;
    m_blurEnabled = true;
    m_borderEnabled = true;
    m_borderColor = QColor(60, 60, 60);
    m_borderThickness = 1;
    m_cornerRadius = 8;
    m_dockHeight = 48;
    m_dockMargin = 10;
    m_iconTheme = "breeze";
    m_showTooltips = true;
    m_runningIndicatorEnabled = true;
    m_trashCommand = "dolphin trash:/";
}

Config::MonitorMode Config::monitorMode() const {
    return m_monitorMode;
}

QStringList Config::specificMonitors() const {
    return m_specificMonitors;
}

Config::VisibilityMode Config::visibilityMode() const {
    return m_visibilityMode;
}

int Config::autoHideDelayMs() const {
    return m_autoHideDelayMs;
}

QColor Config::backgroundColor() const {
    return m_backgroundColor;
}

double Config::opacity() const {
    return m_opacity;
}

bool Config::blurEnabled() const {
    return m_blurEnabled;
}

bool Config::borderEnabled() const {
    return m_borderEnabled;
}

QColor Config::borderColor() const {
    return m_borderColor;
}

int Config::borderThickness() const {
    return m_borderThickness;
}

int Config::cornerRadius() const {
    return m_cornerRadius;
}

int Config::dockHeight() const {
    return m_dockHeight;
}

int Config::dockMargin() const {
    return m_dockMargin;
}

QString Config::iconTheme() const {
    return m_iconTheme;
}

bool Config::showTooltips() const {
    return m_showTooltips;
}

bool Config::runningIndicatorEnabled() const {
    return m_runningIndicatorEnabled;
}

QString Config::trashCommand() const {
    return m_trashCommand;
}

QString Config::defaultConfigPath() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return configDir + "/plasma-dock/config.toml";
}

QString Config::defaultPinnedAppsPath() {
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    return configDir + "/plasma-dock/pinned-apps.toml";
}
