import QtQuick
import QtQuick.Controls

import "../"

Rectangle {
    id: root
    property alias config: dockPanel.config
    property alias dockHeight: dockPanel.dockHeight

    color: config.backgroundColor
    opacity: config.opacity
    radius: config.cornerRadius

    border.color: config.borderEnabled ? config.borderColor : "transparent"
    border.width: config.borderThickness

    layer.enabled: config.blurEnabled
    layer.effect: GaussianBlur {
        radius: 8
        samples: 16
    }

    DockPanel {
        id: dockPanel
        anchors.fill: parent
        anchors.margins: 0
    }
}
