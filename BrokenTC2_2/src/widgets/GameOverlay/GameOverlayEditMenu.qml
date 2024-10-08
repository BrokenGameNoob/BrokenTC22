import QtQuick 2.15

Item {
    id: root
    required property var globalArea
    required property bool editModeEnabled
    required property bool targetComponentHovered
    required property bool targetComponentBeingDragged

    readonly property real hideDelayMs: 1500
    readonly property bool shouldBeVisible: editModeEnabled
                                            && (targetComponentHovered
                                                || hideDelay.running
                                                || mouseIsOnMenu)
                                            && !targetComponentBeingDragged

    default property alias actualContent: content.children
    property alias content: content.children

    height: content.height
    width: content.width
    visible: shouldBeVisible

    property bool mouseIsOnMenu: false

    onTargetComponentHoveredChanged: {
        hideDelay.start()
    }

    MouseArea {
        id: disableGlobalArea
        hoverEnabled: true
        anchors.fill: content
    }

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height
    }

    MouseArea {
        id: enterMenuDetectionArea
        hoverEnabled: true
        enabled: !mouseIsOnMenu
        anchors.fill: content
        onEntered: {
            mouseIsOnMenu = true
        }
    }

    Connections {
        target: globalArea
        function onContainsMouseChanged() {
            if (globalArea.containsMouse) {
                mouseIsOnMenu = false
            }
        }
    }

    Timer {
        id: hideDelay
        interval: root.hideDelayMs
        repeat: false
    }
}
