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
    signal leaveHelpPanel

    RowLayout {
        id: titleRow
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: implicitHeight + QMLStyle.kStandardMargin

        RoundButton {
            Layout.preferredHeight: parent.height - QMLStyle.kStandardMargin
            Layout.preferredWidth: Layout.preferredHeight

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
                leaveHelpPanel()
            }
        }

        LabelledIcon {
            id: titleButton
            Layout.fillWidth: true
            text: qsTr("Help")
            source: Constants.kIconHelp
            iconWidth: Style.kStandardIconWidth
            iconHeight: Style.kStandardIconWidth
            Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
        }
    }

    Rectangle {
        id: separatorH
        anchors {
            top: titleRow.bottom
            left: parent.left
            right: parent.right
        }
        color: QMLStyle.kBorderColor
        opacity: 0.5
        height: 1
    }

    Flickable {
        anchors {
            top: separatorH.bottom
            topMargin: QMLStyle.kStandardMargin
            left: parent.left
            leftMargin: QMLStyle.kStandardMargin
            right: parent.right
            rightMargin: QMLStyle.kStandardMargin
            bottom: parent.bottom
            bottomMargin: QMLStyle.kStandardMargin
        }

        clip: true
        contentHeight: helpContent.implicitHeight

        GridLayout {
            id: helpContent
            rowSpacing: QMLStyle.kStandardMargin
            columnSpacing: QMLStyle.kStandardMargin
            columns: 2
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }

            ColoredImage {
                Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                source: Constants.kIconLogs
                sourceSize.width: QMLStyle.kStandardTitleIconSize
                sourceSize.height: QMLStyle.kStandardTitleIconSize
                color: QMLStyle.kTextColor
                fillMode: Image.PreserveAspectFit
            }

            Button {
                text: qsTr("Open logs folder")
                Layout.fillWidth: true
                onClicked: {
                    Qt.openUrlExternally(ServiceManager.GetLogFolder())
                }
            }

            ColoredImage {
                Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                source: Constants.kRealIconDiscord
                sourceSize.width: QMLStyle.kStandardTitleIconSize
                sourceSize.height: QMLStyle.kStandardTitleIconSize
                color: "#5662F6"
                fillMode: Image.PreserveAspectFit
            }

            Button {
                text: qsTr("Join discord server")
                Layout.fillWidth: true
                readonly property string url: "https://discord.gg/3wH2RjVe4n"
                onClicked: {
                    Qt.openUrlExternally(url)
                }
            }

            ColoredImage {
                Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                source: Constants.kRealIconGithub
                sourceSize.width: QMLStyle.kStandardTitleIconSize
                sourceSize.height: QMLStyle.kStandardTitleIconSize
                color: QMLStyle.kTextColor
                fillMode: Image.PreserveAspectFit
            }

            Button {
                text: qsTr("Open github page")
                Layout.fillWidth: true
                onClicked: {
                    Qt.openUrlExternally(
                                "https://github.com/BrokenGameNoob/BrokenTC22")
                }
            }
        }
    }
}
