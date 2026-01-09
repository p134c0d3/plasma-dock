import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    property alias config: dockPanel.config
    property alias dockHeight: dockPanel.dockHeight

    color: config.backgroundColor
    opacity: config.opacity
    radius: config.cornerRadius

    border.color: config.borderEnabled ? config.borderColor : "transparent"
    border.width: config.borderThickness

    DockPanel {
        id: dockPanel
        anchors.fill: parent
        anchors.margins: 0
    }
}
