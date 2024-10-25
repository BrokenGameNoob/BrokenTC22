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
    property var activeController: ServiceManager.controllerHandler.activeController

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
                topMargin: Style.kStandardMargin
                right: parent.right
                rightMargin: Style.kStandardMargin
            }
            font: QMLStyle.kFontH2Bold
            iconSize: QMLStyle.kStandardTitleIconSize
            iconOnTheRight: false

            source: Constants.kIconController
            text: qsTr("CONTROLLER")
        }

        Item {
            id: leftPanelContent
            /* Left part content */
            anchors {
                left: parent.left
                right: parent.right
                top: leftPanelLabel.bottom
                topMargin: Style.kStandardMargin * 2
                bottom: parent.bottom
            }

            ScrollView {
                id: scrollView
                anchors.fill: parent

                ColumnLayout {
                    id: column
                    anchors {
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                        leftMargin: Style.kStandardMargin * 2
                        right: parent.right
                        rightMargin: Style.kStandardMargin * 2
                    }

                    GroupedEditor {
                        targetElement: root.activeController
                        targetGroup: "global"
                        title: qsTr("GLOBAL")
                        Layout.fillWidth: true
                    }

                    Item {
                        height: Style.kStandardMargin * 3
                    }

                    GroupedEditor {
                        targetElement: root.activeController
                        targetGroup: "soft_controls"
                        title: qsTr("SOFTWARE CONTROLS")
                        Layout.fillWidth: true
                    }

                    Item {
                        height: Style.kStandardMargin * 3
                    }

                    GroupedEditor {
                        targetElement: root.activeController
                        targetGroup: "sequential_gear"
                        title: qsTr("SEQUENTIAL GEARS")
                        Layout.fillWidth: true
                    }

                    Item {
                        height: Style.kStandardMargin * 3
                    }

                    GroupedEditor {
                        targetElement: root.activeController
                        targetGroup: "gear"
                        title: qsTr("DIRECT GEARS")
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
                topMargin: Style.kStandardMargin
                // horizontalCenter: parent.horizontalCenter
                right: parent.right
                rightMargin: Style.kStandardMargin
            }
            font: QMLStyle.kFontH2Bold
            iconSize: QMLStyle.kStandardTitleIconSize
            iconOnTheRight: true

            source: Constants.kIconKeyboard
            text: qsTr("KEYBOARD")
        }

        Item {
            /* Right part content */
            anchors {
                left: parent.left
                right: parent.right
                top: rightPanelLabel.bottom
                topMargin: Style.kStandardMargin * 2
                bottom: parent.bottom
            }

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
                        targetElement: ServiceManager.keyboardHandler.profile
                        targetGroup: "soft_controls"
                        title: qsTr("SOFTWARE CONTROLS")
                        Layout.fillWidth: true
                    }
                }
            }
        }
    }
}
