import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    property Foos.Note note
    property alias expanded: text.visible

    signal selectCategory(var cat)

    color: "white"
    clip: true

    border.color: "red"
    border.width: 1

    height: {
        var ret = categoryText.height;
        if (text.visible)
            ret += text.height;
        return ret;
    }

    Behavior on height {
        NumberAnimation {
            duration: 100
        }
    }

    Text {
        id: categoryText
        width: parent.width
        color: "blue"
        text: note.category.label

        MouseArea {
            anchors.fill: parent
            onClicked: text.visible = !text.visible
        }
    }

    Rectangle {
        anchors.right: categoryText.right
        anchors.verticalCenter: categoryText.verticalCenter
        anchors.rightMargin: 5
        width: 10
        height: 10
        color: "blue"

        MouseArea {
            anchors.fill: parent
            onClicked: root.selectCategory(note.category)
        }
    }

    Text {
        id: text
        visible: false
        width: parent.width
        y: categoryText.height
        color: "green"
        text: note.text
    }

    Loader {
        id: categorySelector
        active: false
        source: "CategorySelector.qml"
        anchors.centerIn: parent
        width: 200
        height: 200
        onItemChanged: {
            if (item !== null) {
                item.show(root.note.category)
            }
        }
    }
}
