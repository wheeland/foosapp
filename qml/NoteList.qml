import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    color: _style.colorBackground

    property Foos.Player player

    QtObject {
        id: d
        property Foos.Note editNote
        readonly property bool editing: (d.editNote !== null)
        readonly property real padding: 4
    }

    function endEditing(save) {
        if (save)
            editNoteItem.save();
        if (d.editing && d.editNote.text.length === 0)
            player.removeNote(d.editNote)
        categorySelector.hide();
        d.editNote = null;
    }

    //
    // Top Menu
    //
    LameTopMenu {
        id: menu
        height: 40 * _scale

        LameMenuButton {
            text: d.editing ? "Cancel" : "Back"
            center: 40 * _scale
            onClicked: {
                endEditing(false);
            }
        }

        LameMenuButton {
            text: d.editing ? "Category" : "Add"
            center: 120 * _scale
            anchors.verticalCenter: parent.verticalCenter
            onClicked: {
                if (!d.editing)
                    d.editNote = player.newNote();
                categorySelector.show(d.editNote.category);
            }
        }

        LameMenuButton {
            visible: d.editing
            text: "Save"
            center: 220 * _scale
            anchors.verticalCenter: parent.verticalCenter
            onClicked: endEditing(true)
        }
    }

    Item {
        id: body
        anchors {
            left: parent.left
            right: parent.right
            top: menu.bottom
            bottom: parent.bottom
        }

        //
        // Scroll View of all notes
        //
        Flickable {
            visible: !d.editing   // MouseArea won't get release event if invisible
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
                        onEditRequested: d.editNote = model.note
                    }
                }
            }
        }

        //
        // Note that is being edited, on top of everything else
        //
        Note {
            id: editNoteItem
            visible: d.editing
            anchors.fill: parent
            anchors.margins: d.padding * _scale
            textFocus: true
            note: d.editNote
            animating: false
        }

        //
        // Select category for new/existing notes
        //
        CategorySelector {
            id: categorySelector
            anchors.centerIn: parent
            width: 200
            height: 200
        }
    }
}
