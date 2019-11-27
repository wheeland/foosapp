import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    property Foos.Note note
    property bool expanded: false
    property real padding: 8
    property bool animating: true
    property bool editing: false
    property alias textFocus: textEdit.focus
    property alias text: textEdit.text

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

        Flickable {
            id: textFlick

            anchors.top: categoryText.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.topMargin: root.padding * _scale

            height: {
                if (root.textFocus)
                    return root.height - 3 * root.padding * _scale - categoryText.height;
                else if (root.expanded)
                    return textEdit.implicitHeight;
                else
                    return textEdit.cappedHeight
            }

            contentWidth: textEdit.width
            contentHeight: textEdit.height
            clip: true

            function ensureVisible(cursor) {
                contentY = Math.min(contentY, cursor.y);
                contentY = Math.max(contentY, cursor.y + cursor.height - height);
            }

            TextEdit {
                id: textEdit

                readonly property real cappedHeight: Math.min(2.5 * font.pixelSize, implicitHeight)

                width: textFlick.width
                height: (root.expanded || focus) ? implicitHeight : cappedHeight
                color: _style.colorText
                font.pixelSize: 11 * _scale
                wrapMode: TextInput.Wrap

                onCursorRectangleChanged: textFlick.ensureVisible(cursorRectangle)
            }
        }
    }

    MouseArea {
        anchors.fill: content
        visible: !root.editing
        onClicked: expandTimer.start()
        pressAndHoldInterval: 250
        onPressAndHold: _controller.goToNoteEdit(root.note)
    }
}
