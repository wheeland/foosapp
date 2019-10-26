import QtQuick 2.11

Rectangle {
    id: root

    width: parent.width

    color: _style.colorMenu

    //
    // Footer Border
    //
    Rectangle {
        id: footer
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        height: 2 * _scale
        color: _style.colorBorder
    }
}
