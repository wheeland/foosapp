import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root
    visible: _controller.verificationOpen

    color: _style.colorMenu
    height: column.childrenRect.height + 24 * _scale

    border.color: _style.colorBorder
    border.width: _scale

    property real childWidth: column.width - 16 * _scale

    Column {
        id: column

        anchors.fill: parent
        anchors.margins: 6 * _scale
        padding: 8 * _scale
        spacing: 8 * _scale

        Text {
            text: _controller.verificationText
            wrapMode: Text.Wrap
            color: _style.colorText
            width: root.childWidth
            font.pixelSize: 12 * _scale
            font.bold: true
        }

        Item {
            height: childrenRect.height
            width: root.childWidth

            LameButton {
                anchors.left: parent.left
                anchors.margins: root.childWidth / 4 - width / 2
                text: "Cancel"
                onClicked: _controller.verificationClicked(false)
            }

            LameButton {
                anchors.right: parent.right
                anchors.margins: root.childWidth / 4 - width / 2
                text: "OK"
                onClicked: _controller.verificationClicked(true)
            }

        }
    }
}
