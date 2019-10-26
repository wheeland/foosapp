import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    property Foos.Note note

    color: "white"

    border.color: "red"
    border.width: 1

    height: categoryText.height

    Text {
        id: categoryText
        width: parent.width
        color: "blue"
        text: note.category.label
    }
}
