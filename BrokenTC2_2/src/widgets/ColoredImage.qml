import QtQuick 2.15
import Qt5Compat.GraphicalEffects

Image {
    id: icon
    property alias color: colorOverlay.color

    ColorOverlay {
        id: colorOverlay
        anchors.fill: parent
        source: parent
        color: "#ff0000" // make image like it lays under red glass
    }
}
