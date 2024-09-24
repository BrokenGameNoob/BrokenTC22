import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../utils"
import "../widgets"

import btc2

Popup {
    id: frame

    property color backgroundColor: QMLStyle.kBackgroundColor
    property color borderColor: QMLStyle.kBackgroundColor
    property real borderWidth: 0
    property color dimmedColor: Qt.darker(QMLStyle.kBackgroundColor, 1.2)
    property real dimmedColorOpacity: 0.9

    default property alias actualContent: frame.contentItem
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    background: Rectangle {
        id: bg
        color: frame.backgroundColor
        radius: QMLStyle.kStandardMargin
        border.color: frame.borderColor
        border.width: frame.borderWidth
    }

    Overlay.modal: Rectangle {
        color: Qt.hsla(frame.dimmedColor.hslHue,
                       frame.dimmedColor.hslSaturation,
                       frame.dimmedColor.hslLightness, frame.dimmedColorOpacity)
    }
}
