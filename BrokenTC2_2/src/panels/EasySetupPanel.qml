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

    property alias selectedGame: gameCombobox.selectedGame
    property var easySetupModel: selectedGame === Game.THE_CREW_2 ? theCrew2 : null

    onVisibleChanged: {
        theCrew2.Cancel()
    }

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
                leaveEasySetupPanel()
            }
        }

        LabelledIcon {
            id: titleButton
            Layout.fillWidth: true
            text: qsTr("Easy setup")
            source: Constants.kIconSetup
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

    ColumnLayout {
        anchors {
            top: separatorH.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            margins: QMLStyle.kStandardMargin
        }

        ComboBox {
            id: gameCombobox
            Layout.fillWidth: true
            Layout.preferredHeight: gameCombobox.implicitHeight * 0.7

            visible: theCrew2.state === EasySetupTheCrew.HOME
                     || root.selectedGame === Game.NONE
            editable: false

            model: ServiceManager.gameSelector.GameSelectionModel(false)

            readonly property var selectedGame: ServiceManager.gameSelector.GetGameFromName(
                                                    currentText)
            Layout.alignment: Qt.AlignTop
        }

        Item {
            id: theCrewWidget
            Layout.fillWidth: true
            Layout.fillHeight: true

            visible: selectedGame === Game.THE_CREW_2

            ColumnLayout {
                anchors.fill: parent
                spacing: 0
                Label {
                    id: tcHomeLabel
                    visible: theCrew2.state === EasySetupTheCrew.HOME
                    font: QMLStyle.kFontH3Bold
                    text: qsTr("Press \"Next\" to start the setup")
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
                Image {
                    visible: tcHomeLabel.visible
                    fillMode: Image.PreserveAspectFit
                    source: theCrew2.game == Game.THE_CREW_2 ? Constants.kRealIconTheCrew2 : Constants.kRealIconTheCrewMotorfist
                    sourceSize.width: width
                    sourceSize.height: height
                    Layout.preferredHeight: parent.height * 0.6
                    Layout.preferredWidth: parent.width * 0.6
                    Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                }

                ColumnLayout {
                    visible: theCrew2.state == EasySetupTheCrew.OPTIONS
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: QMLStyle.kStandardMargin

                    RowLayout {
                        Item {
                            Layout.fillWidth: true
                        }
                        Switch {
                            Layout.fillHeight: true
                            Layout.alignment: Qt.AlignHCenter
                            text: qsTr("Auto set bindings <strong>in game</strong>")
                            checked: theCrew2.autoSetGameBindings
                            onCheckedChanged: theCrew2.autoSetGameBindings = checked
                        }
                        Item {
                            Layout.fillWidth: true
                        }
                    }
                }

                RowLayout {
                    id: tcControlRow
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignBottom
                    Item {
                        Layout.fillWidth: true
                    }
                    Button {
                        visible: theCrew2.state == (EasySetupTheCrew.OPTIONS + 1)
                        text: qsTr("Previous")
                        icon.source: Constants.kIconBackArrow
                        onClicked: theCrew2.Previous()
                    }
                    Button {
                        visible: true
                        text: theCrew2.state == EasySetupTheCrew.FINISHED ? qsTr("Finish") : qsTr(
                                                                                "Next")
                        icon.source: Constants.kIconForwardArrow
                        onClicked: theCrew2.Next()
                    }
                }
            }
        }
    }

    /* Setup models */
    EasySetupTheCrew {
        id: theCrew2
        game: Game.THE_CREW_2
    }
}
