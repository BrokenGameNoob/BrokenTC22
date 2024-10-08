import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../../utils"
import "../../widgets"

import btc2

MouseArea {
    id: gearIndicatorEditArea
    anchors.fill: gearIndicatorEditRow
    hoverEnabled: true
    propagateComposedEvents: true

    required property var enableDragComponent
    required property var globalArea

    property real switchOffDelayMs: 4000
    property bool privateHasMouseIn: false
    property bool externalTrigger: false
    readonly property bool hasMouseIn: gearIndicatorEditArea.privateHasMouseIn
    readonly property bool shouldDisplayMenu: gearIndicatorEditArea.hasMouseIn
                                              || enableDragComponent.containsMouse
                                              || halfSingleShotTimer.running

    function switchOn() {
        singleShotTimer.startTimer()
        externalTrigger = true
        console.log("Switch on")
    }
    onEntered: {
        if (!singleShotTimer.running) {
            console.log("Entered without external trigger")
            return
        }
        gearIndicatorEditArea.privateHasMouseIn = true
        console.log("HAS MOUSE IN")
    }

    Rectangle {
        anchors.fill: parent
        color: "green"
        opacity: 0.5
    }

    Connections {
        target: globalArea
        function onContainsMouseChanged() {
            if (globalArea.containsMouse) {
                gearIndicatorEditArea.privateHasMouseIn = false
            }
        }
    }

    Connections {
        target: enableDragComponent
        function onContainsMouseChanged() {
            gearIndicatorEditArea.switchOn()
        }
    }

    Timer {
        id: singleShotTimer
        interval: gearIndicatorEditArea.switchOffDelayMs
        repeat: false
        property int startTime: 0
        onTriggered: {
            gearIndicatorEditArea.externalTrigger = false
        }
        function startTimer() {
            startTime = Date.now()
            singleShotTimer.start()
            halfSingleShotTimer.start()
        }
        function getRemainingTime() {
            if (singleShotTimer.running) {
                var elapsed = Date.now() - startTime
                return singleShotTimer.interval - elapsed
            } else {
                return 0
            }
        }
    }
    Timer {
        id: halfSingleShotTimer
        interval: gearIndicatorEditArea.switchOffDelayMs / 2.
        repeat: false
        onTriggered: {
            gearIndicatorEditArea.externalTrigger = false
        }
    }
}
