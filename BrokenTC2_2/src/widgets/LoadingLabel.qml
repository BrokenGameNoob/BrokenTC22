import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15

import btc2 1.0

Label {
    id: root

    property string implicitText: ""
    property real animationDuration: 500
    readonly property real maxWidth: textMetrics.width

    elide: Text.ElideMiddle

    TextMetrics {
        id: textMetrics
        text: root.implicitText + "..."
        font: root.font
    }

    SequentialAnimation on text {
        loops: Animation.Infinite
        PropertyAnimation {
            to: root.implicitText + ".  "
            duration: root.animationDuration
        }
        PropertyAnimation {
            to: root.implicitText + ".. "
            duration: root.animationDuration
        }
        PropertyAnimation {
            to: root.implicitText + "..."
            duration: root.animationDuration
        }
    }
}
