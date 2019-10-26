import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    property Foos.Note note
    property bool expanded: false
    property real padding: 8
    property alias textFocus: text.focus

    signal selectCategory(var cat)
    signal requestEdit()

    color: _style.colorArea
    clip: true

    border.color: _style.colorBorder
    border.width: _scale

    height: content.height + 2 * padding * _scale

    Item {
        id: content
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: root.padding * _scale

        height: childrenRect.height
        clip: true

        Behavior on height {
            NumberAnimation {
                duration: 100
            }
        }

        Text {
            id: categoryText
            color: text.focus ? _style.colorTextHighlighted : _style.colorText
            text: root.note ? note.category.label : ""
            font.pixelSize: 12 * _scale
            font.bold: true
        }

        TextEdit {
            id: text
            anchors.top: categoryText.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: root.padding * _scale
            height: (root.expanded || focus) ? implicitHeight
                                             : 2.5 * font.pixelSize
            color: _style.colorText
            font.pixelSize: 11 * _scale
            wrapMode: TextInput.Wrap
            text: root.note ? root.note.text : ""
        }

        Rectangle {
            anchors.right: parent.right
            anchors.verticalCenter: categoryText.verticalCenter
            anchors.rightMargin: 5
            width: 10
            height: 10
            color: "blue"

            MouseArea {
                anchors.fill: parent
                onClicked: root.selectCategory(root.note.category)
            }
        }
    }

    MouseArea {
        anchors.fill: content
        onClicked: root.expanded = !root.expanded
        onDoubleClicked: root.requestEdit()
    }
}
