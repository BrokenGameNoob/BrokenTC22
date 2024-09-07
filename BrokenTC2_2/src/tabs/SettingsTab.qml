import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15

import ".."
import "../utils"
import "../widgets"

import btc2

Item {
    GroupedEditor {
        targetElement: ServiceManager.settings
        targetGroup: "overlay"
        title: qsTr("OVERLAY")

        anchors {
            top: parent.top
            topMargin: QMLStyle.kStandardMargin
            left: parent.left
            leftMargin: QMLStyle.kStandardMargin
            right: parent.right
            rightMargin: QMLStyle.kStandardMargin
        }
    }
}
