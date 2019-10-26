import QtQuick 2.11

Rectangle {
    id: root

    property alias text: text.text

    signal clicked()
    signal pressAndHold()

    color: mouse.pressed ? _style.colorMenuClicked : _style.colorMenuButton

    border.color: _style.colorBorder
    border.width: _scale

    width: text.width + 10 * _scale
    height: text.height + 8 * _scale

    Text {
        id: text
        font.pixelSize: 12.0 * _scale
        anchors.centerIn: parent
        color: _style.colorText
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked()
        onPressAndHold: root.pressAndHold()
    }
}
