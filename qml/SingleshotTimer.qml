import QtQuick 2.11

SequentialAnimation {
    property alias delay: delayAnim.duration
    property alias action: scriptAction.script

    PauseAnimation { id: delayAnim; duration: 0 }
    ScriptAction { id: scriptAction }
}
