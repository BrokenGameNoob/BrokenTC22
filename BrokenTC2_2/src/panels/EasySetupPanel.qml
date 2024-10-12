import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15
import "../tabs"
import "../debug"
import "../utils"
import "../widgets"
import ".."
import "."

import btc2 1.0

Item {
    id: root
    signal leaveEasySetupPanel

    readonly property var selectedGame: ServiceManager.gameSelector.selectedGame
    property var easySetupModel: selectedGame === Game.THE_CREW_2 ? theCrew2 : null

    RoundButton {
        id: controllerListLeaveButton
        anchors {
            left: parent.left
            leftMargin: Style.kStandardMargin
            top: titleButton.top
            topMargin: Style.kStandardMargin / 2
            bottom: titleButton.bottom
            bottomMargin: Style.kStandardMargin / 2
        }

        width: height

        contentItem: Item {
            width: parent.width * 0.8
            height: parent.height * 0.8
            ColoredImage {
                source: Constants.kIconBackArrow
                sourceSize.width: parent.width
                sourceSize.height: parent.height
                color: QMLStyle.kIconColor
            }
        }

        onClicked: {
            leaveEasySetupPanel()
        }
    }

    TabButton {
        id: titleButton
        text: qsTr("Easy setup")
        icon.source: Constants.kIconSetup
        icon.width: Style.kStandardIconWidth
        icon.height: Style.kStandardIconWidth

        checkable: false

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: {
            leaveEasySetupPanel()
        }
    }

    EasySetupInterface {
        id: theCrew2
    }
}
