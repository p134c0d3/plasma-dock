import QtQuick

Rectangle {
    id: root
    width: 400
    height: 48
    color: "#2b2b2b"
    border.color: "#444"
    border.width: 1
    radius: 8

    Text {
        anchors.centerIn: parent
        text: "Plasma Dock v1.0"
        color: "#fff"
    }
}