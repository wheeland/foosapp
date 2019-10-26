import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    color: _style.colorBackground

    property Foos.Player player

    QtObject {
        id: d
        property Foos.Note editNote
        readonly property bool editing: (editNote !== null)
        readonly property real padding: 4
    }

    //
    // Scroll View of all notes
    //
    Flickable {
        visible: !d.editing
        anchors.fill: parent

        contentHeight: noteColumn.height
        contentWidth: noteColumn.width

        Column {
            id: noteColumn
            width: root.width
            spacing: d.padding * _scale
            padding: d.padding * _scale

            Repeater {
                model: root.player
                delegate: Note {
                    width: root.width - 2 * d.padding * _scale
                    note: model.note

                    onSelectCategory: categorySelector.show(cat)
                    onRequestEdit: d.editNote = model.note
                }
            }
        }
    }

    //
    // Note that is being edited, on top of everything else
    //
    Item {
        id: editItem
        visible: d.editing
        anchors.fill: parent

        LameTopMenu {
            id: editMenu
            height: 50 * _scale

            LameMenuButton {
                text: "Save"
                x: 50 * _scale
                y: 20 * _scale
                onClicked: d.editNote = null
            }
        }

        Note {
            anchors.fill: parent
            anchors.topMargin: editMenu.height
            textFocus: true
            note: d.editNote
            animating: false
        }

    }

    CategorySelector {
        id: categorySelector
        anchors.centerIn: parent
        width: 200
        height: 200
    }
}
