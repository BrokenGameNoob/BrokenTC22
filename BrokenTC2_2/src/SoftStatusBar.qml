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

    // Game selection combo
    ComboBox {
        id: gameCombobox

        anchors {
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: visible ? Style.kStandardMargin : 0
        }

        visible: false

        editable: false
        width: visible ? 200 : 0
        height: implicitHeight * 0.7
        model: ServiceManager.gameSelector.GameSelectionModel()

        onActivated: {
            ServiceManager.gameSelector.SetSelectionModelSelectedGame(
                        currentText)
        }
    }

    // Controller selection combo
    ComboBox {
        id: controllerCombobox

        anchors {
            verticalCenter: parent.verticalCenter
            left: gameCombobox.right
            leftMargin: Style.kStandardMargin
        }

        editable: false
        width: 200
        height: implicitHeight * 0.7
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
            ServiceManager.controllerHandler.SetActiveController(currentText)
        }
    }
    RoundButton {
        id: controllerListEditButton
        enabled: !easySetupButton.checked
        anchors {
            verticalCenter: parent.verticalCenter
            left: controllerCombobox.right
            leftMargin: Style.kStandardMargin
        }
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
        anchors {
            verticalCenter: parent.verticalCenter
            left: controllerListEditButton.right
            leftMargin: Style.kStandardMargin
        }
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
        anchors {
            verticalCenter: parent.verticalCenter
            left: easySetupButton.right
            leftMargin: Style.kStandardMargin
        }
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

    Label {
        id: gearLabel

        anchors {
            verticalCenter: parent.verticalCenter
            right: parent.right
            rightMargin: Style.kStandardMargin
        }

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

    //    Label {
    //        anchors {
    //            verticalCenter: parent.verticalCenter
    //            right: gearLabel.left
    //            rightMargin: Style.kStandardMargin
    //        }

    //        text: ServiceManager.gearHandler.gearModeStr
    //    }
    Button {
        anchors {
            verticalCenter: parent.verticalCenter
            right: gearLabel.left
            rightMargin: Style.kStandardMargin
        }

        text: ServiceManager.gearHandler.gearModeStr
        onClicked: {
            ServiceManager.gearHandler.CycleMode()

            //            console.info(ServiceManager.tmp.actions.count)
            //            ServiceManager.tmp.actions[0].key += 1
            ServiceManager.PublishOverlayNotification("HEY", 1000)
        }
    }
    Label {
        id: dummyLabel
    }
}
