import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15
import "./tabs"
import "./debug"
import "."

import btc2

ApplicationWindow {
    id: root

    //    Material.theme: Material.Dark
    readonly property real baseRatioToStandard: 0.5
    readonly property int baseWidth: 1920 * baseRatioToStandard
    readonly property int baseHeight: 1080 * baseRatioToStandard + (300 * ServiceManager.hasDebInfo)

    property var gearHandler: ServiceManager.gearHandler

    title: qsTr("BrokenTC2") + " [" + ServiceManager.versionStr + "]"
    width: baseWidth
    height: baseHeight
    visible: true

    SplitView {
        id: splitView
        anchors.fill: parent
        orientation: Qt.Vertical

        Item {
            SplitView.minimumHeight: 0.1 * root.baseHeight
            SplitView.preferredHeight: 0.6 * root.baseHeight

            TabBar {
                id: bar
                width: parent.width

                //        currentIndex: 2
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

            StackLayout {
                id: mainContent
                anchors {
                    top: bar.bottom
                    bottom: statusBar.top
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

            SoftStatusBar {
                id: statusBar
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }
            }
        }

        /* Splitview members */
        DebugWindow {
            id: debugWindow

            SplitView.minimumHeight: 0.3 * root.baseHeight
            SplitView.preferredHeight: 300

            visible: ServiceManager.hasDebInfo
        }
    }

    Component.onCompleted: {
        ServiceManager.OnMainWindowLoaded()
    }
}
