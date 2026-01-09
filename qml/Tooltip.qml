import QtQuick
import QtQuick.Controls

ToolTip {
    id: root
    property string tooltipText: ""

    text: tooltipText
    visible: tooltipText && tooltipText.length > 0
    delay: 500
    timeout: 3000

    contentItem: Text {
        text: root.text
        color: "#ffffff"
        font.pixelSize: 12
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        color: "#333333"
        opacity: 0.9
        radius: 4
        border.color: "#555555"
        border.width: 1
    }
}
