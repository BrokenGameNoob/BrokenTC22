import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15
import "./tabs"
import "./debug"
import "./utils"
import "./panels"
import "./widgets"
import "."

import btc2

ApplicationWindow {
    id: root

    property var overlayWindow: null

    //    Material.theme: Material.Dark
    readonly property real baseRatioToStandard: 0.5
    readonly property int baseWidth: 1920 * baseRatioToStandard
    readonly property int baseHeight: 1080 * baseRatioToStandard + (300 * ServiceManager.hasDebInfo)

    property var gearHandler: ServiceManager.gearHandler

    title: qsTr("BrokenTC2") + " [" + ServiceManager.versionStr + "]"
    width: baseWidth
    height: baseHeight
    visible: true

    minimumHeight: 200 + (300 * ServiceManager.hasDebInfo)
    minimumWidth: 800

    Rectangle {
        anchors.fill: parent
        visible: ServiceManager.settings.BackgroundImage.length != 0
        opacity: ServiceManager.settings.BackgroundImageOpacity
        Image {
            anchors.fill: parent
            id: backgroundImage
            source: ServiceManager.settings.BackgroundImage
            fillMode: Image.PreserveAspectCrop
        }
    }

    SplitView {
        id: splitView
        anchors.fill: parent
        orientation: Qt.Vertical

        Item {
            id: panels
            SplitView.minimumHeight: 0.1 * root.baseHeight
            SplitView.preferredHeight: 0.6 * root.baseHeight

            readonly property bool mainPanelFullDisplay: !statusBar.activateControllerListPanel
                                                         && !statusBar.activateEasySetupPanel
            readonly property bool otherPanelsStillVisile: controllerListPanel.visible
                                                           || easySetupPanel.visible

            MainPanel {
                id: mainPanel
                width: panels.mainPanelFullDisplay ? parent.width : 0
                visible: width > 0
                clip: true

                Behavior on width {
                    enabled: !panels.mainPanelFullDisplay
                             || panels.otherPanelsStillVisile
                    NumberAnimation {
                        easing {
                            type: Easing.InOutQuad
                            amplitude: 1.0
                            period: 0.2
                        }
                    }
                }

                anchors {
                    top: parent.top
                    left: parent.left
                    bottom: statusBar.top
                }
            }

            ControllerListPanel {
                id: controllerListPanel
                width: statusBar.activateControllerListPanel ? parent.width - mainPanel.width : 0
                visible: width > 0

                onLeaveControllerListPanel: {
                    statusBar.deactivateControllerListPanel()
                }

                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: statusBar.top
                }
            }

            EasySetupPanel {
                id: easySetupPanel
                width: statusBar.activateEasySetupPanel ? parent.width - mainPanel.width : 0
                visible: width > 0

                onLeaveEasySetupPanel: {
                    statusBar.deactivateEasySetupPanel()
                }

                anchors {
                    top: parent.top
                    right: parent.right
                    bottom: statusBar.top
                }
            }

            SoftStatusBar {
                id: statusBar
                anchors {
                    bottom: parent.bottom
                    left: parent.left
                    right: parent.right
                }

                onOverlayButtonCheckedChanged: {
                    overlayWindow.editModeEnabled = overlayButtonChecked
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

        // Instantiate and show the overlay window
        var overlayComponent = Qt.createComponent(
                    "widgets/GameOverlay/GameOverlay.qml")
        if (overlayComponent.status === Component.Ready) {
            overlayWindow = overlayComponent.createObject(null)
            if (overlayWindow === null) {
                console.error("Error creating overlay window")
            }
        } else {
            console.error("Error loading overlay window component",
                          overlayComponent.errorString())
        }

        if (ServiceManager.settings.LaunchStartProcedure) {
            statusBar.openEasySetupPanel()
            introductionPopup.open()
        }
    }

    Connections {
        target: overlayWindow
        function onLeaveEditMode() {
            statusBar.overlayButtonChecked = false
        }
    }

    onClosing: {
        if (overlayWindow !== null) {
            overlayWindow.allowClose = true
            overlayWindow.close()
        }
    }

    ThemedPopup {
        id: introductionPopup

        width: Overlay.overlay.width / 1.5
        height: Overlay.overlay.height / 1.5

        ColumnLayout {
            anchors.fill: parent
            spacing: QMLStyle.kStandardMargin

            Item {
                Layout.minimumHeight: QMLStyle.kStandardMargin
                Layout.fillHeight: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: QMLStyle.kStandardMargin
                Item {
                    Layout.fillWidth: true
                }

                ColoredImage {
                    id: popupIcon
                    Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                    Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                    source: Constants.kIconUbiNope
                    color: "black"
                    readonly property real animationDuration: 300
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation {
                            to: "red"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "green"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "blue"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "yellow"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "purple"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "cyan"
                            duration: popupIcon.animationDuration
                        }
                        ColorAnimation {
                            to: "magenta"
                            duration: popupIcon.animationDuration
                        }
                    }
                }

                Text {
                    font: QMLStyle.kFontH2Bold
                    text: qsTr("Welcome to BrokenTC2!")
                    horizontalAlignment: Text.AlignHCenter
                    color: QMLStyle.kTextColor
                }

                ColoredImage {
                    Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                    Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                    source: Constants.kIconUbiNope
                    color: "white"
                    SequentialAnimation on color {
                        loops: Animation.Infinite
                        ColorAnimation {
                            to: "red"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "green"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "blue"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "yellow"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "purple"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "cyan"
                            duration: popupIcon.animationDuration * 1.6
                        }
                        ColorAnimation {
                            to: "magenta"
                            duration: popupIcon.animationDuration * 1.6
                        }
                    }
                }

                Item {
                    Layout.fillWidth: true
                }
            }

            Item {
                Layout.preferredHeight: QMLStyle.kStandardMargin * 2
            }
            Text {
                Layout.fillWidth: true
                font: QMLStyle.kFontH3
                text: qsTr("Please follow the setup to get started.\nIf you are doubting about an option, just leave the default value!")
                horizontalAlignment: Text.AlignHCenter
                color: QMLStyle.kTextColor
                wrapMode: Text.WordWrap
            }

            ColoredImage {
                Layout.fillHeight: true
                Layout.fillWidth: true
                source: Constants.kIconFast
                color: QMLStyle.kTextColor
                fillMode: Image.PreserveAspectFit
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 0
                Item {
                    Layout.fillWidth: true
                }

                ButtonOk {
                    onClicked: {
                        introductionPopup.close()
                    }
                }

                Item {
                    Layout.preferredWidth: QMLStyle.kStandardMargin
                }
            }
        }
    }
}
