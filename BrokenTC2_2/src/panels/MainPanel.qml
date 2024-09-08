import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15
import "../tabs"
import "../debug"
import "../utils"
import "."
import ".."

import btc2

Item {
    TabBar {
        id: bar
        width: parent.width

        currentIndex: ServiceManager.settings.OpenedTab
        onCurrentIndexChanged: {
            ServiceManager.settings.OpenedTab = currentIndex
        }

        TabButton {
            id: refTabButton
            text: qsTr("Software controls")
            icon.source: Constants.kIconController
            icon.width: Style.kStandardIconWidth
            icon.height: Style.kStandardIconWidth
        }
        TabButton {
            text: qsTr("Game configuration")
            icon.source: Constants.kIconKeyboard
            icon.width: refTabButton.icon.width
            icon.height: refTabButton.icon.height
        }
        TabButton {
            text: qsTr("Settings")
            icon.source: Constants.kIconSettings
            icon.width: refTabButton.icon.width
            icon.height: refTabButton.icon.height
        }
    }

    Rectangle {
        id: separatorH
        anchors {
            top: bar.bottom
            left: parent.left
            right: parent.right
        }
        color: QMLStyle.kBorderColor
        opacity: 0.5
        height: 1
    }

    StackLayout {
        id: mainContent
        anchors {
            top: bar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        currentIndex: bar.currentIndex

        SoftwareControlsTab {}

        Item {
            id: discoverTab
        }

        SettingsTab {}
    }
}
