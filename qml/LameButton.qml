import QtQuick 2.11

Rectangle {
    id: root

    property alias text: text.text

    property color baseColor: "#222222"
    property color clickColor: "#555555"
    property color borderColor: "#cccccc"
    property color textColor: "#ffffff"

    signal clicked()
    signal pressAndHold()

    color: mouse.pressed ? clickColor : baseColor

    border.color: borderColor
    border.width: 2

    width: text.width + 10
    height: text.height + 8

    Text {
        id: text
        font.pixelSize: 12.0
        anchors.centerIn: parent
        color: root.textColor
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked()
        onPressAndHold: root.pressAndHold()
    }
}
