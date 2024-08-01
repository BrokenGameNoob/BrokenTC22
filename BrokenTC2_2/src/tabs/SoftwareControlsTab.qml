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
    /* Left part */
    Item {
        anchors {
            left: parent.left
            right: separator.left
            top: parent.top
            bottom: parent.bottom
        }

        LabelledIcon {
            id: leftPanelLabel
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            source: Constants.kIconController
            text: qsTr("Controller commands")
        }

        Item {
            id: leftPanelContent
            /* Left part content */
            anchors {
                left: parent.left
                right: parent.right
                top: leftPanelLabel.bottom
                topMargin: Style.kStandardMargin
                bottom: parent.bottom
            }

            ScrollView {
                id: scroll
                anchors.fill: parent
                anchors.leftMargin: QMLStyle.kStandardMargin
                anchors.rightMargin: QMLStyle.kStandardMargin
                anchors.bottomMargin: QMLStyle.kStandardMargin

                contentWidth: scroll.width
                contentHeight: column.height
                clip: true
                ColumnLayout {
                    id: column

                    anchors.fill: parent

                    GroupedEditor {
                        targetElement: ServiceManager.dummy
                        targetGroup: "global"
                        title: qsTr("GLOBAL")

                        Layout.fillWidth: true
                    }

                    GroupedEditor {
                        targetElement: ServiceManager.dummy
                        targetGroup: "gear"
                        title: qsTr("GEAR")

                        Layout.fillWidth: true
                    }
                }
            }
        }
    }

    /* Separator */
    Rectangle {
        id: separator
        anchors {
            top: parent.top
            bottom: parent.bottom
            horizontalCenter: parent.horizontalCenter

            topMargin: Style.kStandardMargin
            bottomMargin: Style.kStandardMargin
        }

        color: QMLStyle.kBorderColor
        width: 1
        border.color: QMLStyle.kBorderColor
    }

    /* Right part */
    Item {
        anchors {
            left: separator.right
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }

        LabelledIcon {
            id: rightPanelLabel
            anchors {
                top: parent.top
                horizontalCenter: parent.horizontalCenter
            }

            source: Constants.kIconKeyboard
            text: qsTr("Keyboard commands")
        }

        Item {
            /* Left part content */
            anchors {
                left: parent.left
                right: parent.right
                top: rightPanelLabel.bottom
                topMargin: Style.kStandardMargin
                bottom: parent.bottom
            }
        }
    }

    //    Button {
    //        id: button
    //        anchors.centerIn: parent
    //        text: qsTr("Hey")

    //        MessageDialog {
    //            id: messagebox
    //            buttons: MessageDialog.Cancel
    //            text: "The document has been modified."
    //        }

    //        onClicked: {
    //            //messagebox.open()
    //            //ServiceManager.test()
    //            console.log(gearHandler.gearModeStr)
    //            gearHandler.CycleMode()
    //            console.log(gearHandler.gearModeStr)
    //        }
    //    }
    //    Button {
    //        id: buttonDown
    //        anchors {
    //            right: button.horizontalCenter
    //            top: button.bottom
    //            rightMargin: 4
    //            topMargin: 4
    //        }

    //        text: qsTr("Gear Down")

    //        onClicked: {
    //            console.log("Before: " + gearHandler.gear)
    //            gearHandler.GearDown()
    //            console.log("After: " + gearHandler.gear)
    //        }
    //    }
    //    Button {
    //        id: buttonUp
    //        anchors {
    //            left: button.horizontalCenter
    //            top: button.bottom
    //            leftMargin: 4
    //            topMargin: 4
    //        }

    //        text: qsTr("Gear Up")

    //        onClicked: {
    //            console.log("Before: " + gearHandler.gear)
    //            gearHandler.GearUp()
    //            console.log("After: " + gearHandler.gear)
    //        }
    //    }
    //    Switch {
    //        anchors.topMargin: 2
    //        anchors.top: button.bottom
    //        text: qsTr("Wi-Fi")
    //    }
}
