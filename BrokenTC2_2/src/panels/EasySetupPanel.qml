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

        /* The Crew handling */
        Item {
            id: theCrewWidget
            Layout.fillWidth: true
            Layout.fillHeight: true

            visible: selectedGame === Game.THE_CREW_2
                     || selectedGame === Game.THE_CREW_MOTORFIST

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

                ColumnLayout {
                    visible: theCrew2.state
                             == EasySetupTheCrew.WORK_IN_PROGRESS_WAITING_GAME_CLOSING
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: QMLStyle.kStandardMargin

                    Item {
                        Layout.fillHeight: true
                    }

                    LoadingIcon {
                        Layout.fillWidth: false
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: QMLStyle.kStandardTitleIconSize * 2
                        Layout.preferredWidth: Layout.preferredHeight
                        backgroundColor: QMLStyle.kAccentColor
                        foregroundColor: QMLStyle.kPrimaryColor
                    }
                    LoadingLabel {
                        font: QMLStyle.kFontH3Bold
                        implicitText: qsTr("Waiting for the game to close")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

                ColumnLayout {
                    visible: theCrew2.state == EasySetupTheCrew.WORK_IN_PROGRESS_EDITING
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: QMLStyle.kStandardMargin

                    Item {
                        Layout.fillHeight: true
                    }

                    LoadingIcon {
                        Layout.fillWidth: false
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: QMLStyle.kStandardTitleIconSize * 2
                        Layout.preferredWidth: Layout.preferredHeight
                        backgroundColor: QMLStyle.kAccentColor
                        foregroundColor: QMLStyle.kPrimaryColor
                    }
                    LoadingLabel {
                        font: QMLStyle.kFontH3Bold
                        implicitText: qsTr("Editing configuration files")
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        Layout.fillWidth: true
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

                ColumnLayout {
                    visible: theCrew2.state == EasySetupTheCrew.FINISHED
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    spacing: QMLStyle.kStandardMargin

                    Item {
                        Layout.fillHeight: true
                    }

                    LabelledIcon {
                        id: tcSuccessLabel
                        font: QMLStyle.kFontH3Bold
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: implicitWidth
                        states: [
                            State {
                                when: theCrew2.successState === EasySetupTheCrew.SUCCESS
                                PropertyChanges {
                                    tcSuccessLabel {
                                        text: qsTr(
                                                  "Easy setup ran successfully")
                                        source: Constants.kIconOk
                                        iconColor: QMLStyle.kOkColor
                                    }
                                }
                            },
                            State {
                                when: theCrew2.successState === EasySetupTheCrew.FAILURE
                                PropertyChanges {
                                    tcSuccessLabel {
                                        text: qsTr("Failed to run easy setup")
                                        source: Constants.kIconCancel
                                        iconColor: QMLStyle.kCancelColor
                                    }
                                }
                            },
                            State {
                                when: theCrew2.successState
                                      === EasySetupTheCrew.UNKNOWN_SUCCESS_STATE
                                PropertyChanges {
                                    tcSuccessLabel {
                                        text: qsTr("Unknown success state...")
                                        source: Constants.kIconCancel
                                        iconColor: QMLStyle.kCancelColor
                                    }
                                }
                            }
                        ]
                    }

                    Item {
                        Layout.fillHeight: true
                    }
                }

                RowLayout {
                    id: tcControlRow
                    enabled: theCrew2.state
                             !== EasySetupTheCrew.WORK_IN_PROGRESS_WAITING_GAME_CLOSING
                             && theCrew2.state !== EasySetupTheCrew.WORK_IN_PROGRESS_EDITING
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
        game: root.selectedGame === Game.THE_CREW_2
              || root.selectedGame === Game.THE_CREW_MOTORFIST ? root.selectedGame : Game.THE_CREW_2
    }
}
