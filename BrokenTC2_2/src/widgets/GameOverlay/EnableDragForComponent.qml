import QtQuick 2.15

Item {
    id: root
    required property var target
    required property real minScale
    required property real maxScale

    property alias containsMouse: dragArea.containsMouse
    property alias beingDragged: dragArea.drag.active

    MouseArea {
        id: dragArea
        anchors.fill: parent
        drag.target: root.target
        hoverEnabled: true
        cursorShape: Qt.OpenHandCursor
    }

    WheelHandler {
        id: wheelHandler
        onWheel: wheel => {
                     var newScale = root.target.scale + wheel.angleDelta.y / 1200.0
                     root.target.scale = Math.max(root.minScale,
                                                  Math.min(root.maxScale,
                                                           newScale))
                 }
    }
}
