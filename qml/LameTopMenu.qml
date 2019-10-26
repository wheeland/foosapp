import QtQuick 2.11

Rectangle {
    id: root

    color: "#e5e5e5"

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

        Row {
            id: editButtonRow

            LameButton {
                text: "Change Category"
            }
        }

        Note {
            anchors.fill: parent
            anchors.topMargin: 100
            note: d.editNote
            textFocus: true
        }

    }

    CategorySelector {
        id: categorySelector
        anchors.centerIn: parent
        width: 200
        height: 200
    }
}
