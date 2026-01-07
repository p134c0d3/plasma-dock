#include <QGuiApplication>
#include <QQuickWindow>
#include <QScreen>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QTimer>

#include "config.h"
#include "dockwindow.h"
#include "monitor.h"

int main(int argc, char *argv[]) {
    QGuiApplication app(argc, argv);
    app.setApplicationName("plasma-dock");
    app.setOrganizationName("plasma-dock");

    QCommandLineParser parser;
    parser.setApplicationDescription("Plasma Dock - A lightweight Wayland-native dock for KDE Plasma");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption timeoutOption(
        QStringList() << "t" << "timeout",
        "Exit after specified seconds (for testing)",
        "seconds",
        "0"
    );
    parser.addOption(timeoutOption);

    parser.process(app);

    int timeout = parser.value(timeoutOption).toInt();

    Config config;
    config.load();

    QList<DockWindow*> dockWindows;

    Config::MonitorMode mode = config.monitorMode();
    QStringList specificMonitors = config.specificMonitors();

    QList<QScreen*> screens = QGuiApplication::screens();
    for (QScreen *screen : screens) {
        bool shouldShow = false;

        switch (mode) {
        case Config::AllMonitors:
            shouldShow = true;
            break;
        case Config::PrimaryMonitor:
            shouldShow = (screen == QGuiApplication::primaryScreen());
            break;
        case Config::SpecificMonitors:
            shouldShow = specificMonitors.contains(screen->name());
            break;
        }

        if (shouldShow) {
            DockWindow *dock = new DockWindow(screen, &config);
            dockWindows.append(dock);
        }
    }

    QObject::connect(&app, &QGuiApplication::screenAdded, [&](QScreen *screen) {
        bool shouldShow = false;

        switch (mode) {
        case Config::AllMonitors:
            shouldShow = true;
            break;
        case Config::PrimaryMonitor:
            shouldShow = (screen == QGuiApplication::primaryScreen());
            break;
        case Config::SpecificMonitors:
            shouldShow = specificMonitors.contains(screen->name());
            break;
        }

        if (shouldShow) {
            DockWindow *dock = new DockWindow(screen, &config);
            dockWindows.append(dock);
        }
    });

    QObject::connect(&app, &QGuiApplication::screenRemoved, [&](QScreen *screen) {
        for (DockWindow *dock : dockWindows) {
            if (dock->screen() == screen) {
                dockWindows.removeAll(dock);
                dock->deleteLater();
                break;
            }
        }
    });

    if (timeout > 0) {
        QTimer::singleShot(timeout * 1000, [&]() {
            app.quit();
        });
    }

    int result = app.exec();

    for (DockWindow *dock : dockWindows) {
        delete dock;
    }

    return result;
}
