import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../../utils"
import "../../widgets"

import btc2

Window {
    id: btc2Overlay
    visible: true
    x: 0
    y: 0
    width: Screen.width
    height: Screen.height
    color: "transparent"
    flags: Qt.WindowStaysOnTopHint | Qt.FramelessWindowHint | Qt.Tool
           | (!editModeEnabled ? Qt.WindowTransparentForInput : 0)
    visibility: Window.FullScreen

    signal leaveEditMode

    property bool editModeEnabled: false
    property bool allowClose: false

    readonly property real minScale: 0.6
    readonly property real maxScale: 2.5

    onClosing: function (event) {
        if (!allowClose) {
            event.accepted = false // Prevent the window from closing
        } else {
            event.accepted = true
        }
    }
    onEditModeEnabledChanged: {
        if (editModeEnabled) {
            btc2Overlay.requestActivate()
        }
    }

    function leaveIfEditMode() {
        if (editModeEnabled) {
            leaveEditMode()
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            console.log("ESC pressed")
            leaveIfEditMode()
        }
    }

    Item {
        anchors.fill: parent

        MouseArea {
            id: globalArea
            anchors.fill: parent
            hoverEnabled: true
            preventStealing: false
            propagateComposedEvents: true
        }

        Rectangle {
            width: 100
            height: 100
            color: editModeEnabled ? QMLStyle.kBackgroundColor : "transparent"
            anchors.fill: parent
            opacity: 0.8
        }

        Label {
            id: editModeLabel
            visible: editModeEnabled
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Place your elements where you want (drag & drop)") + "\n" + qsTr(
                      "Press ESC to leave")
            background: Rectangle {
                color: "black"
                opacity: 0.5
                radius: QMLStyle.kRadius
                anchors.centerIn: parent
                width: parent.width + QMLStyle.kStandardMargin
                height: parent.height + QMLStyle.kStandardMargin
            }
        }
        Button {
            id: resetButton
            text: qsTr("Reset to default")
            visible: editModeEnabled
            anchors {
                top: editModeLabel.bottom
                topMargin: QMLStyle.kStandardMargin
                horizontalCenter: editModeLabel.horizontalCenter
            }
        }
        RoundButton {
            id: leaveEditButton
            anchors {
                right: resetButton.left
                rightMargin: Style.kStandardMargin
                verticalCenter: resetButton.verticalCenter
            }
            width: height
            visible: editModeEnabled

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
                leaveIfEditMode()
            }
        }

        Label {
            id: gearLabel

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height - QMLStyle.kStandardMargin - height

            x: defaultX
            y: defaultY

            FontMetrics {
                id: fm
                font: gearLabel.font
            }
            font.pointSize: 50
            font.bold: true

            width: implicitWidth + QMLStyle.kStandardMargin
            padding: Style.kStandardMargin / 2
            topPadding: Style.kStandardMargin / 2 - (0.12 * font.pixelSize)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            text: ServiceManager.gearHandler.gearStr

            background: Rectangle {
                id: gearFrame
                color: "black"
                // border.color: QMLStyle.kBorderColor
                radius: width > height ? height / 2 : width / 2
                opacity: 0.3
            }

            EnableDragForComponent {
                id: dragGearLabel
                anchors.fill: parent
                target: parent
                minScale: btc2Overlay.minScale
                maxScale: btc2Overlay.maxScale
            }
        }

        GameOverlayEditMenu {
            globalArea: globalArea
            editModeEnabled: btc2Overlay.editModeEnabled
            targetComponentHovered: dragGearLabel.containsMouse

            anchors {
                verticalCenter: gearLabel.verticalCenter
                left: gearLabel.right
                leftMargin: Style.kStandardMargin
            }

            Row {
                id: gearIndicatorEditRow
                height: true ? implicitHeight : 0
                enabled: true

                Slider {
                    from: btc2Overlay.minScale
                    to: btc2Overlay.maxScale
                    value: gearLabel.scale
                    orientation: Qt.Vertical
                    height: gearLabel.height * 1.5

                    onValueChanged: {
                        gearLabel.scale = value
                    }
                }
            }
        }
    }
}
