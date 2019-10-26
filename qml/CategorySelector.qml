import QtQuick 2.11

import Foos 1.0 as Foos

Rectangle {
    id: root

    visible: d.category !== null

    QtObject {
        id: d
        property Foos.Category category
        property int stage
        property var options
        property int optionCount
        readonly property real padding: 4

        function next(index) {
            if (stage === 0) {
                category.primary = index;
                options = category.validTechniques(index);
                stage = 1;
            }
            else if (stage === 1) {
                category.technique = index;
                options = category.validSubTechniques(category.primary, index);
                stage = 2;
            }
            else if (stage == 2) {
                category.subTechnique = index;
                options = []
                stage = 3;
                category = null;
                root.accepted();
            }

            optionCount = d.options.length;
            if (optionCount == 1 && stage < 3)
                next(0);
        }
    }

    signal accepted()
    signal cancelled()

    function show(cat) {
        if (cat !== null) {
            d.category = cat;
            d.stage = 0;
            d.options = cat.validPrimaries();
            d.optionCount = d.options.length;
        }
    }

    function hide() {
        d.category = null;
        d.stage = 0;
    }

    color: _style.colorMenu
    clip: true

    border.color: _style.colorBorder
    border.width: _scale

    Flickable {
        id: flickable
        anchors.fill: parent

        contentHeight: content.height
        contentWidth: content.width

        Column {
            id: content
            width: flickable.width
            spacing: d.padding * _scale
            padding: d.padding * _scale

            Repeater {
                id: repeater
                model: d.optionCount
                delegate: LameButton {
                    width: content.width - 2 * d.padding * _scale
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: {
                        if (d.category == null)
                            return "";
                        if (d.stage === 0)
                            return d.category.primaryName(d.options[index]);
                        else if (d.stage === 1)
                            return d.category.techniqueName(d.options[index]);
                        else if (d.stage === 2)
                            return d.category.subTechniqueName(d.options[index]);
                        else
                            return "";
                    }
                    onClicked: d.next(index)
                }
            }
        }
    }
}
