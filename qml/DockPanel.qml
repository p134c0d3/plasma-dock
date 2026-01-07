import QtQuick
import QtQuick.Layouts
import QtQml

Item {
    id: root
    property var config: null
    property int dockHeight: 48

    readonly property alias pinnedApps: _pinnedApps
    readonly property alias runningApps: _runningApps

    ListModel {
        id: _pinnedApps
    }

    ListModel {
        id: _runningApps
    }

    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 8
        anchors.rightMargin: 8
        spacing: 4

        Flow {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 4
            layoutDirection: Qt.LeftToRight

            Repeater {
                model: _pinnedApps
                IconButton {
                    height: root.dockHeight - 16
                    iconSource: model.icon
                    tooltipText: model.name
                    showTooltip: root.config ? root.config.showTooltips : true
                    isRunning: false
                }
            }

            Rectangle {
                width: 1
                height: root.dockHeight - 24
                color: "#555"
                opacity: 0.5
                visible: _runningApps.count > 0
            }

            Repeater {
                model: _runningApps
                IconButton {
                    height: root.dockHeight - 16
                    iconSource: model.icon
                    tooltipText: model.name
                    showTooltip: root.config ? root.config.showTooltips : true
                    isRunning: true
                }
            }
        }

        Item {
            width: root.dockHeight
            height: root.dockHeight

            IconButton {
                anchors.centerIn: parent
                width: root.dockHeight - 16
                height: root.dockHeight - 16
                iconSource: "user-trash-full"
                tooltipText: "Trash"
                showTooltip: root.config ? root.config.showTooltips : true
                onClicked: {
                    Qt.openUrlExternally("trash:/")
                }
            }
        }
    }
}
