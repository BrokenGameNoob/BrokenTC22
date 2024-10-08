import QtQuick 2.15

Item {
    required property var globalArea
    default property alias actualContent: content.children

    width: content.implicitWidth
    height: content.implicitHeight

    MouseArea {
        id: blockingArea
        anchors.fill: content
    }

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height
    }
}
