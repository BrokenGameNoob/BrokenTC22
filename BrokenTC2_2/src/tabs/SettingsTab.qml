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

    ScrollView {
        id: scrollViewRight
        anchors.fill: parent

        ColumnLayout {
            id: columnRight
            anchors {
                top: parent.top
                topMargin: QMLStyle.kStandardMargin
                bottom: parent.bottom
                bottomMargin: QMLStyle.kStandardMargin
                left: parent.left
                leftMargin: Style.kStandardMargin * 2
                right: parent.right
                rightMargin: Style.kStandardMargin * 2
            }

            property var conflictNamesList: ServiceManager.keyboardProfileConflicts

            GroupedEditor {
                targetElement: ServiceManager.settings
                targetGroup: "overlay"
                title: qsTr("OVERLAY")
                Layout.fillWidth: true
            }

            Item {
                height: Style.kStandardMargin * 3
            }

            GroupedEditor {
                targetElement: ServiceManager.settings
                targetGroup: "soft"
                title: qsTr("SOFTWARE")
                Layout.fillWidth: true
            }

            Item {
                height: Style.kStandardMargin * 3
            }

            GroupedEditor {
                targetElement: ServiceManager.settings
                targetGroup: "tts"
                title: qsTr("VOICE ANNOUCEMENTS")
                Layout.fillWidth: true
            }

            Item {
                height: Style.kStandardMargin * 3
            }
        }
    }
}
