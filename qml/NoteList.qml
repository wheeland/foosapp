import QtQuick 2.11

import Foos 1.0 as Foos

Column {
    id: root

    property Foos.Player player

    Repeater {
        model: root.player
        delegate: Note {
            width: root.width
            note: model.note
        }
    }
}
