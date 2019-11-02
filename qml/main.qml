import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    color: "black"

    border.color: "white"
    border.width: 3

    //
    // Top Menu
    //
    LameTopMenu {
        id: menu
        height: 40 * _scale

        Repeater {
            model: _controller.menuModel

            delegate: LameMenuButton {
                text: model.text
                center: model.position * _scale
                onClicked: _controller.menuClicked(index)
                padding: 12
            }
        }

        LameMenuButton {
            visible: (_controller.currentPage === Foos.Controller.StartPage)
            text: "Logging"
            center: 200 * _scale
            onClicked: logOutput.isVisible = !logOutput.isVisible
            padding: 12
        }
    }

    //
    // Content item with different sub-pages
    //
    Rectangle {
        id: body

        anchors {
            left: parent.left
            right: parent.right
            top: menu.bottom
            bottom: parent.bottom
        }

        color: _style.colorArea
        clip: true

        //
        // show debug messages
        //
        Rectangle {
            id: logOutput
            anchors.fill: parent
            color: _style.colorArea
            property bool isVisible: false
            visible: (_controller.currentPage === Foos.Controller.StartPage) && isVisible

            Flickable {
                anchors.fill: parent
                contentHeight: logOutputColumn.height
                contentWidth: logOutputColumn.width
                Column {
                    id: logOutputColumn
                    width: childrenRect.width
                    height: childrenRect.height
                    Repeater {
                        model: logOutput.visible ? _logger : null
                        delegate: Text {
                            text: model.text
                        }
                    }
                }
            }
        }

        //
        // Player List
        //
        PlayerList {
            anchors.fill: parent
            anchors.margins: 10
            visible: (_controller.currentPage === Foos.Controller.PlayersList)
        }

        //
        // Notes for one player
        //
        NoteList {
            anchors.fill: parent
            anchors.margins: 10
            visible: (_controller.currentPage === Foos.Controller.PlayerView)
            player: _controller.viewedPlayer
            title: player ? (player.firstName + " " + player.lastName) : ""
        }

        //
        // Player Name Entry
        //
        PlayerNamePopup {
            id: playerNamePopup
            visible: (_controller.currentPage === Foos.Controller.PlayerNameEdit)
            anchors.horizontalCenter: parent.horizontalCenter
            y: 20 * _scale
            width: 200 * _scale

            onVisibleChanged: {
                if (visible)
                    startEdit();
                else
                    stopEdit();
            }
        }

        //
        // Note that is being edited
        //
        Note {
            id: editNoteItem
            visible: (_controller.currentPage === Foos.Controller.NoteEdit)
            anchors.fill: parent
            anchors.margins: 10

            property bool isEditing: true
            textFocus: visible && isEditing && !categorySelector.visible
            note: _controller.editedNote
            expanded: true
            animating: false
            editing: true

            onVisibleChanged: {
                isEditing = visible;
                text = visible ? _controller.editedNote.text : "";
            }

            Connections {
                target: _controller
                onEndNoteEditing: {
                    editNoteItem.isEditing = false;
                    _controller.noteEdited(editNoteItem.text)
                }
            }

            //
            // Select category for new/existing notes
            //
            CategorySelector {
                id: categorySelector
                anchors.horizontalCenter: parent.horizontalCenter
                y: 20 * _scale
                width: 200 * _scale
                height: 200 * _scale

                Connections {
                    target: _controller
                    onShowCategorySelector: categorySelector.show(category)
                }
            }
        }
    }
}
