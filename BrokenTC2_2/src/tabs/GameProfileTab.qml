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
                bottom: parent.bottom
                left: parent.left
                leftMargin: Style.kStandardMargin * 2
                right: parent.right
                rightMargin: Style.kStandardMargin * 2
            }

            GroupedEditor {
                targetElement: ServiceManager.gameProfilesHandler.gameProfile
                targetGroup: "gear"
                title: qsTr("GENERAL CONTROLS")
                Layout.fillWidth: true
            }

            Item {
                height: Style.kStandardMargin * 3
            }

            GroupedEditor {
                targetElement: ServiceManager.gameProfilesHandler.gameProfile
                targetGroup: "gear_seq"
                title: qsTr("GROUND VEHICLE")
                Layout.fillWidth: true
            }
        }
    }
}