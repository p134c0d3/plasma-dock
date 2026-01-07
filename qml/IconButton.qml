import QtQuick
import QtQuick.Controls

Item {
    id: root
    property string iconSource: ""
    property string tooltipText: ""
    property bool showTooltip: true
    property bool isRunning: false
    property bool isActive: false
    property int iconSize: 32
    signal clicked()

    width: iconSize + 12
    height: iconSize + 12

    Rectangle {
        anchors.fill: parent
        radius: 4
        color: "transparent"

        Image {
            id: iconImage
            anchors.centerIn: parent
            width: iconSize
            height: iconSize
            source: root.iconSource
            sourceSize.width: iconSize
            sourceSize.height: iconSize
            smooth: true
        }

        Rectangle {
            id: indicator
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 2
            anchors.horizontalCenter: parent.horizontalCenter
            width: 6
            height: 6
            radius: 3
            color: "#4CAF50"
            opacity: root.isRunning && root.config && root.config.runningIndicatorEnabled ? 1 : 0
        }

        Rectangle {
            anchors.fill: parent
            color: "white"
            opacity: root.isActive ? 0.1 : 0
            radius: 4
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onClicked: root.clicked()
            onEntered: {
                showTooltipTimer.start()
                hoverScale.to = 1.1
                hoverScale.start()
            }
            onExited: {
                showTooltipTimer.stop()
                hideTooltip()
                hoverScale.to = 1.0
                hoverScale.start()
            }
        }

        SequentialAnimation {
            id: hoverScale
            PropertyAnimation {
                target: parent
                property: "scale"
                to: 1.0
                duration: 100
            }
        }

        Timer {
            id: showTooltipTimer
            interval: 500
            onTriggered: {
                showTooltip()
            }
        }
    }

    Tooltip {
        id: tooltip
        text: root.tooltipText
        visible: false
    }

    function showTooltip() {
        if (!root.showTooltip || root.tooltipText === "") return;
        tooltip.x = root.x + root.width / 2 - tooltip.width / 2
        tooltip.y = root.y - tooltip.height - 8
        tooltip.visible = true
    }

    function hideTooltip() {
        tooltip.visible = false
    }
}
