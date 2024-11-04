import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import "./tabs"
import "./debug"
import "./utils"
import "./widgets"
import "."

import btc2

Item {
    id: root
    height: 60

    readonly property bool activateControllerListPanel: controllerListEditButton.checked
    function deactivateControllerListPanel() {
        controllerListEditButton.checked = false
    }

    readonly property bool activateEasySetupPanel: easySetupButton.checked
    function deactivateEasySetupPanel() {
        easySetupButton.checked = false
    }

    property alias overlayButtonChecked: overlayButton.checked

    // Separator
    Rectangle {
        anchors {
            bottom: parent.top
            left: parent.left
            right: parent.right

            leftMargin: Style.kStandardMargin / 2
            rightMargin: Style.kStandardMargin / 2
        }

        color: QMLStyle.kBorderColor
        height: 1
        border.color: QMLStyle.kBorderColor
    }

    Label {
        id: dummyLabel
    }

    RowLayout {
        anchors.fill: parent
        spacing: 0

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin
        }

        // Game selection combo
        ComboBox {
            id: gameCombobox

            visible: false

            editable: false
            Layout.preferredWidth: visible ? 200 : 0
            Layout.preferredHeight: implicitHeight * 0.7
            model: ServiceManager.gameSelector.GameSelectionModel()

            onActivated: {
                ServiceManager.gameSelector.SetSelectionModelSelectedGame(
                            currentText)
            }
        }

        // Controller selection combo
        ComboBox {
            id: controllerCombobox

            editable: false
            Layout.preferredWidth: 200
            Layout.preferredHeight: implicitHeight * 0.7
            model: ServiceManager.controllerHandler.controllerList

            readonly property var activeController: ServiceManager.controllerHandler.activeController
            onActiveControllerChanged: {
                controllerCombobox.currentIndex = controllerCombobox.find(
                            activeController.Name)
            }
            onModelChanged: {
                controllerCombobox.currentIndex = controllerCombobox.find(
                            activeController.Name)
            }

            onActivated: {
                ServiceManager.controllerHandler.SetActiveController(
                            currentText)
            }
        }

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin / 2
        }

        RoundButton {
            id: controllerListEditButton
            enabled: !easySetupButton.checked
            checkable: true
            checked: false

            contentItem: Item {
                width: QMLStyle.kStandardTitleIconSize * 0.8
                height: QMLStyle.kStandardTitleIconSize * 0.8
                ColoredImage {
                    id: controllerListIcon
                    source: Constants.kIconControllerList
                    sourceSize.width: parent.width
                    sourceSize.height: parent.height
                    color: parent.parent.checked ? QMLStyle.kAccentColor : QMLStyle.kIconColor
                }
            }
        }

        RoundButton {
            id: easySetupButton
            enabled: !controllerListEditButton.checked
            checkable: true
            checked: false
            contentItem: Item {
                width: QMLStyle.kStandardTitleIconSize * 0.8
                height: QMLStyle.kStandardTitleIconSize * 0.8
                ColoredImage {
                    source: Constants.kIconSetup
                    sourceSize.width: parent.width
                    sourceSize.height: parent.height
                    color: parent.parent.checked ? QMLStyle.kAccentColor : QMLStyle.kIconColor
                }
            }
        }

        RoundButton {
            id: overlayButton
            checkable: true
            checked: false
            contentItem: Item {
                width: QMLStyle.kStandardTitleIconSize * 0.8
                height: QMLStyle.kStandardTitleIconSize * 0.8
                ColoredImage {
                    source: Constants.kIconOverlay
                    sourceSize.width: parent.width
                    sourceSize.height: parent.height
                    color: parent.parent.checked ? QMLStyle.kAccentColor : QMLStyle.kIconColor
                }
            }
        }

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin
        }

        Item {
            Layout.fillWidth: true
        }

        LabelledIcon {
            id: keyboardConflictsLabel
            font: QMLStyle.kFontH3Bold
            iconSize: modeButton.height * 0.7
            iconOnTheRight: true
            iconColor: QMLStyle.kErrorRed
            spacerWidth: QMLStyle.kStandardMargin

            source: Constants.kIconCancel
            text: qsTr("Keyboard conflicts")
            visible: ServiceManager.hasKeyboardConflicts
        }

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin
        }

        Button {
            id: modeButton

            text: ServiceManager.gearHandler.gearModeStr
            onClicked: {
                ServiceManager.gearHandler.CycleMode()
                ServiceManager.PublishOverlayNotification("HEY", 1000)
            }
        }

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin
        }

        Label {
            id: gearLabel

            FontMetrics {
                id: fm
                font: gearLabel.font
            }
            font.pointSize: dummyLabel.font.pointSize * 2
            font.bold: true

            width: fm.height
            padding: Style.kStandardMargin / 2
            topPadding: Style.kStandardMargin / 2 - (0.12 * font.pixelSize)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: ServiceManager.gearHandler.gearStr

            background: Rectangle {
                id: gearFrame
                color: "transparent"
                border.color: QMLStyle.kBorderColor
                radius: height / 2
            }
        }

        Item {
            Layout.preferredWidth: QMLStyle.kStandardMargin
        }
    }
}
