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

        //        anchors {
        //            top: bar.bottom
        //            bottom: parent.bottom
        //            left: parent.left
        //            right: parent.right
        //        }
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
                    text: qsTr("Software controls")
                    icon.source: "qrc:/icons/icon_controller.svg"
                }
                TabButton {
                    text: qsTr("In game controls")
                    icon.source: "qrc:/icons/icon_keyboard.svg"
                }
                TabButton {
                    text: qsTr("Settings")
                    icon.source: "qrc:/icons/icon_settings.svg"
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

                Item {
                    id: homeTab

                    Button {
                        id: button
                        anchors.centerIn: parent
                        text: qsTr("Hey")

                        MessageDialog {
                            id: messagebox
                            buttons: MessageDialog.Cancel
                            text: "The document has been modified."
                        }

                        onClicked: {
                            //messagebox.open()
                            //ServiceManager.test()
                            console.log(gearHandler.gearModeStr)
                            gearHandler.CycleMode()
                            console.log(gearHandler.gearModeStr)
                        }
                    }
                    Button {
                        id: buttonDown
                        anchors {
                            right: button.horizontalCenter
                            top: button.bottom
                            rightMargin: 4
                            topMargin: 4
                        }

                        text: qsTr("Gear Down")

                        onClicked: {
                            console.log("Before: " + gearHandler.gear)
                            gearHandler.GearDown()
                            console.log("After: " + gearHandler.gear)
                        }
                    }
                    Button {
                        id: buttonUp
                        anchors {
                            left: button.horizontalCenter
                            top: button.bottom
                            leftMargin: 4
                            topMargin: 4
                        }

                        text: qsTr("Gear Up")

                        onClicked: {
                            console.log("Before: " + gearHandler.gear)
                            gearHandler.GearUp()
                            console.log("After: " + gearHandler.gear)
                        }
                    }
                    Switch {
                        anchors.topMargin: 2
                        anchors.top: button.bottom
                        text: qsTr("Wi-Fi")
                    }
                }
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
}
