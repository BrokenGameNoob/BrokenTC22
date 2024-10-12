import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../utils"
import "../widgets"

import btc2

Button {
    text: qsTr("Cancel")

    icon.source: Constants.kIconCancel
    icon.color: QMLStyle.kCancelColor
}
