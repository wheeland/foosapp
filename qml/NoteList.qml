import QtQuick 2.11

import Foos 1.0 as Foos

Item {
    id: root

    property Foos.Player player

    Column {
        Repeater {
            model: root.player
            delegate: Note {
                width: root.width
                note: model.note

                onSelectCategory: categorySelector.show(cat)
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
