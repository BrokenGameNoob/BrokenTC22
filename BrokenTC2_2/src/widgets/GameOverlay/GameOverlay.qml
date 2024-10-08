import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../../utils"
import "../../widgets"

import btc2

Window {
    id: root
    visible: ServiceManager.settings.OverlayEnabled
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

    property var overlayModel: ServiceManager.gameOverlay

    onClosing: function (event) {
        if (!allowClose) {
            event.accepted = false // Prevent the window from closing
        } else {
            event.accepted = true
        }
    }
    onEditModeEnabledChanged: {
        if (editModeEnabled) {
            root.requestActivate()
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
        id: content
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
            opacity: 0.9
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
            onClicked: {
                content.resetGearLabel()
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

        function resetGearLabel() {
            overlayModel.GearX = 0
            overlayModel.GearY = 0
            overlayModel.GearScaling = 1
        }

        Label {
            id: gearLabel

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height - QMLStyle.kStandardMargin - height

            readonly property real implicitX: overlayModel.GearX
            readonly property real implicitY: overlayModel.GearY

            x: implicitX === 0 ? defaultX : implicitX
            y: implicitY === 0 ? defaultY : implicitY

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
            visible: overlayModel.GearEnabled || editModeEnabled

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
                minScale: root.minScale
                maxScale: root.maxScale
                onDropped: {
                    overlayModel.GearX = gearLabel.x
                    overlayModel.GearY = gearLabel.y
                }
            }

            property bool inhibitScaleUpdate: false
            Binding {
                target: gearLabel
                property: "scale"
                value: overlayModel.GearScaling
            }
            onScaleChanged: {
                inhibitScaleUpdate = true
                overlayModel.GearScaling = scale
                inhibitScaleUpdate = false
            }
        }

        GameOverlayEditMenu {
            globalArea: globalArea
            editModeEnabled: root.editModeEnabled
            targetComponentHovered: dragGearLabel.containsMouse
            targetComponentBeingDragged: dragGearLabel.beingDragged

            anchors {
                verticalCenter: gearLabel.verticalCenter
                left: gearLabel.right
                leftMargin: Style.kStandardMargin
            }

            Row {
                id: gearIndicatorEditRow
                height: true ? implicitHeight : 0
                enabled: true

                ColumnLayout {
                    id: gearEditorColumnLeft
                    readonly property real maxHeight: gearLabel.height * 1.5
                    height: gearEditorColumnLeft.maxHeight
                    spacing: 0
                    Slider {
                        id: gearScaleSlider
                        from: root.minScale
                        to: root.maxScale
                        value: gearLabel.scale
                        orientation: Qt.Vertical

                        onValueChanged: {
                            gearLabel.scale = value
                        }

                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredHeight: gearEditorColumnLeft.maxHeight
                                                - resetGearButton.height
                    }
                    RoundButton {
                        id: resetGearButton
                        contentItem: Item {
                            width: QMLStyle.kStandardTitleIconSize * 0.8
                            height: QMLStyle.kStandardTitleIconSize * 0.8
                            ColoredImage {
                                source: Constants.kIconBackArrow
                                sourceSize.width: parent.width
                                sourceSize.height: parent.height
                                color: parent.parent.checked ? QMLStyle.kAccentColor : QMLStyle.kIconColor
                            }
                        }
                        Layout.alignment: Qt.AlignHCenter
                    }
                }

                ScrollView {
                    id: gearScrollView
                    width: 200
                    height: gearEditorColumnLeft.height
                    background: Rectangle {
                        color: QMLStyle.kBackgroundColor
                        anchors.fill: parent
                        radius: QMLStyle.kRadius
                        border.color: QMLStyle.kBorderColor
                        border.width: 1
                    }

                    ColumnLayout {
                        id: gearEditorColumn
                        anchors.fill: parent
                        anchors.margins: Style.kStandardMargin

                        GroupedEditor {
                            targetElement: root.overlayModel
                            targetGroup: "gear"
                            title: qsTr("Gear indicator")
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                    }
                }
            }
        }
    }
}
