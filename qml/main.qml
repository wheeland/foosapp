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
            }
        }
    }

    Text {
        text: _controller.currentPage
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
            visible: (_controller.currentPage === Foos.Controller.NotesList)
            player: _controller.viewedPlayer
            title: player ? (player.firstName + " " + player.lastName) : ""

            PlayerNamePopup {
                id: playerNamePopup
                visible: false
                anchors.centerIn: parent
                width: 200 * _scale

                Connections {
                    target: _controller
                    onShowEditPlayerName: playerNamePopup.startEdit()
                }
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
            textFocus: visible && !categorySelector.visible
            note: _controller.editedNote
            expanded: true
            animating: false

            //
            // Select category for new/existing notes
            //
            CategorySelector {
                id: categorySelector
                anchors.centerIn: parent
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
