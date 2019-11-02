import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    height: column.childrenRect.height + 20 * _scale

    color: _style.colorMenu
    clip: true

    border.color: _style.colorBorder
    border.width: _scale

    function startEdit() {
        input1.focus = true;
        input1.text = _controller.viewedPlayer.firstName
        input2.text = _controller.viewedPlayer.lastName
    }

    function stopEdit() {
        input1.focus = false;
        input2.focus = false;
    }

    Column {
        id: column

        anchors.fill: parent
        anchors.margins: 6 * _scale
        padding: 4 * _scale
        spacing: 4 * _scale

        Text {
            text: "First Name"
            color: _style.colorText
            font.pixelSize: 13 * _scale
            font.bold: true
        }

        Rectangle {
            width: column.width - 8 * _scale
            height: 25 * _scale
            color: _style.colorMenu
            border.color: _style.colorBorder
            border.width: _scale

            TextInput {
                id: input1
                font.pixelSize: 11 * _scale
                anchors.fill: parent
                anchors.margins: 5 * _scale
                onAccepted: input2.focus = true
                onTextChanged: _controller.viewedPlayer.firstName = text
            }
        }

        Item { width: 1; height: 10 * _scale }

        Text {
            text: "Last Name"
            color: _style.colorText
            font.pixelSize: 13 * _scale
            font.bold: true
        }

        Rectangle {
            width: column.width - 8 * _scale
            height: 25 * _scale
            color: _style.colorMenu
            border.color: _style.colorBorder
            border.width: _scale

            TextInput {
                id: input2
                font.pixelSize: 11 * _scale
                anchors.fill: parent
                anchors.margins: 5 * _scale
                onAccepted: input2.focus = false
                onTextChanged: _controller.viewedPlayer.lastName = text
            }
        }
    }
}
