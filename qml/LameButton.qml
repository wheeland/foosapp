import QtQuick 2.11

Rectangle {
    id: root

    property alias text: text.text

    property color baseColor: _style.colorArea
    property color clickColor: _style.colorClick
    property color borderColor: _style.colorBorder
    property color textColor: _style.colorText

    signal clicked()
    signal pressAndHold()

    color: mouse.pressed ? clickColor : baseColor

    border.color: borderColor
    border.width: _scale

    width: text.width + 10 * _scale
    height: text.height + 8 * _scale

    Text {
        id: text
        font.pixelSize: 12.0 * _scale
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
