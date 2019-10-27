import QtQuick 2.11

Rectangle {
    id: root

    property alias text: text.text

    signal clicked()
    signal pressAndHold()

    color: mouse.pressed ? _style.colorMenuClicked : _style.colorMenuButton

    border.color: _style.colorBorder
    border.width: _scale

    property real center
    property real padding: 10
    x: center - width / 2
    width: text.width + padding * _scale
    height: text.height + padding * _scale

    anchors.verticalCenter: parent.verticalCenter

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
