import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    property Foos.Note note
    property bool expanded: false
    property real padding: 8
    property bool animating: true
    property alias textFocus: textEdit.focus

    SingleshotTimer {
        id: expandTimer
        action: root.expanded = !root.expanded
        delay: 100
    }

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
                duration: root.animating ? 100 : 0
            }
        }

        Text {
            id: categoryText
            color: textEdit.focus ? _style.colorTextHighlighted : _style.colorText
            text: root.note ? note.category.label : ""
            font.pixelSize: 12 * _scale
            font.bold: true
        }

        TextEdit {
            id: textEdit
            anchors.top: categoryText.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: root.padding * _scale
            height: (root.expanded || focus) ? implicitHeight
                                             : Math.min(2.5 * font.pixelSize, implicitHeight)
            color: _style.colorText
            font.pixelSize: 11 * _scale
            wrapMode: TextInput.Wrap
            text: root.note ? root.note.text : ""
            onTextChanged: {
                if (root.note)
                    root.note.text = text;
            }
        }
    }

    MouseArea {
        anchors.fill: content
        visible: !root.textFocus
        onClicked: expandTimer.start()
        onDoubleClicked: _controller.goToNoteEdit(root.note)
    }
}
