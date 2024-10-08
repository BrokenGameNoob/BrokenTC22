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
    minimumWidth: 700

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
}
