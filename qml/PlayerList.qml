import QtQuick 2.11

import Foos 1.0 as Foos

//
// Scroll View of all players
//
ListView {
    id: root

    model: _controller.database
    spacing: 2 * _scale

    delegate: Rectangle {
        id: playerItem

        color: playerMouseArea.pressed ? _style.colorClick : _style.colorArea

        border.color: _style.colorBorder
        border.width: _scale

        width: root.width
        height: playerText.height + 4 * _scale

        Text {
            id: playerText
            color: _style.colorText
            anchors.verticalCenter: parent.verticalCenter
            x: 4 * _scale
            text: model.player.firstName
            font.pixelSize: 12 * _scale
            font.bold: true
        }

        MouseArea {
            id: playerMouseArea
            anchors.fill: parent
            onClicked: _controller.goToPlayerView(model.player)
        }
    }
}
