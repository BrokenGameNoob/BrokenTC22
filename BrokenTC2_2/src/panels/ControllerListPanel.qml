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

import btc2

Item {
    signal leaveControllerListPanel

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
            leaveControllerListPanel()
        }
    }

    TabButton {
        id: titleButton
        text: qsTr("Controllers list")
        icon.source: Constants.kIconControllerList
        icon.width: Style.kStandardIconWidth
        icon.height: Style.kStandardIconWidth

        checkable: false

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: {
            leaveControllerListPanel()
        }
    }

    Rectangle {
        id: separatorH
        anchors {
            top: titleButton.bottom
            left: parent.left
            right: parent.right
        }
        color: QMLStyle.kBorderColor
        opacity: 0.5
        height: 1
    }

    ListView {
        anchors {
            left: parent.left
            right: parent.right
            top: separatorH.bottom
            topMargin: Style.kStandardMargin
            bottom: parent.bottom
        }

        model: ServiceManager.controllerHandler.knownControllersProfiles

        delegate: Text {
            anchors {
                left: parent.left
                right: parent.right
            }
            text: modelData.Name
            font: QMLStyle.kFontH4Bold
            color: QMLStyle.kTextColor
        }
    }
}
