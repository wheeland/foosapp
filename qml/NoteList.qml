import QtQuick 2.11

import Foos 1.0 as Foos

Item {
    id: root

    property Foos.Player player

    Flickable {
        anchors.fill: parent

        contentHeight: noteColumn.height
        contentWidth: noteColumn.width
        flickableDirection: Flickable.VerticalFlick

        FocusScope {
            width: parent.width
            height: noteColumn.height
            focus: true

            Column {
                id: noteColumn
                width: parent.width

                Repeater {
                    model: root.player
                    delegate: Note {
                        width: root.width
                        note: model.note

                        onSelectCategory: categorySelector.show(cat)
                    }
                }
            }
        }
    }

    CategorySelector {
        id: categorySelector
        anchors.centerIn: parent
        width: 200
        height: 200
    }
}
