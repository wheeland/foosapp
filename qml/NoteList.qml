import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    color: _style.colorBackground

    property Foos.Player player
    property alias title: titleText.text

    Flickable {
        anchors.fill: parent
        contentHeight: noteColumn.height
        contentWidth: noteColumn.width
        interactive: (contentHeight > height)
        flickableDirection: Flickable.VerticalFlick

        Column {
            id: noteColumn
            width: root.width
            height: childrenRect.height
            spacing: 4 * _scale
            padding: 4 * _scale

            Text {
                id: titleText
                color: _style.colorText
                font.pixelSize: 13 * _scale
                font.bold: true
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Repeater {
                model: Foos.NotesSortModel {
                    sourceModel: root.player
                    dynamicSortFilter: true
                }

                delegate: Note {
                    width: root.width - 8 * _scale
                    note: model.note
                    text: model.note ? model.note.text : ""
                }
            }
        }
    }
}
