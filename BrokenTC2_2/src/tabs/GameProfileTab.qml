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
    id: root
    property var conflictList: ServiceManager.gameProfileConflicts
    property bool hasConflicts: ServiceManager.hasKeyboardConflicts

    property var gameProfile: ServiceManager.gameProfilesHandler.gameProfile

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
            onWidthChanged: {

                // console.log("width changed: " + width)
            }

            Item {
                height: Style.kStandardMargin
            }

            Rectangle {
                color: "transparent"
                Layout.fillWidth: true
                Layout.preferredHeight: gameCombobox.implicitHeight * 0.7

                Text {
                    text: qsTr("Game")
                    font: QMLStyle.kFontH3
                    color: QMLStyle.kTextColor
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                        right: parent.horizontalCenter
                    }
                }

                ComboBox {
                    id: gameCombobox

                    visible: true
                    editable: false
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.horizontalCenter
                        right: parent.right
                    }

                    model: ServiceManager.gameSelector.GameSelectionModel(false)

                    onActivated: {
                        ServiceManager.gameSelector.SetSelectionModelSelectedGame(
                                    currentText)
                    }

                    Component.onCompleted: {
                        ServiceManager.gameSelector.SetSelectionModelSelectedGame(
                                    currentText)
                    }

                    currentIndex: model.indexOf(
                                      ServiceManager.settings.SelectedGameName)
                }
            }

            Item {
                height: Style.kStandardMargin
            }

            GroupedEditor {
                targetElement: root.gameProfile
                targetGroup: "behavior"
                title: qsTr("BEHAVIOR")
                Layout.fillWidth: true
                conflictedPropertiesName: root.conflictList
            }

            Item {
                height: Style.kStandardMargin * 3
            }

            GroupedEditor {
                targetElement: root.gameProfile
                targetGroup: "gear"
                title: qsTr("GENERAL CONTROLS")
                Layout.fillWidth: true
                conflictedPropertiesName: root.conflictList
            }

            Item {
                height: Style.kStandardMargin * 3
            }

            GroupedEditor {
                targetElement: root.gameProfile
                targetGroup: "gear_seq"
                title: qsTr("GROUND VEHICLE")
                Layout.fillWidth: true
                conflictedPropertiesName: root.conflictList
            }
        }
    }
}
