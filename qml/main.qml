import QtQuick 2.11

Rectangle {
    id: root

    color: "black"

    border.color: "white"
    border.width: 3

    NoteList {
        anchors.fill: parent
        anchors.margins: 10
        player: _controller.myself
    }
}
