import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../../utils"
import "../../widgets"

import btc2

Item {
    id: root
    required property var globalArea
    required property bool editModeEnabled
    required property var targetEnableDragForComponent
    required property var targetComponent
    required property real minScale
    required property real maxScale

    signal resetWanted

    property real minimumHeight: 100

    readonly property real hideDelayMs: 1000
    readonly property bool shouldBeVisible: editModeEnabled
                                            && (targetComponentHovered
                                                || hideDelay.running
                                                || mouseIsOnMenu)
                                            && !targetComponentBeingDragged

    default property alias actualContent: gearEditorColumn.data
    property alias content: gearEditorColumn.data

    readonly property bool targetComponentHovered: targetEnableDragForComponent.containsMouse
    readonly property bool targetComponentBeingDragged: targetEnableDragForComponent.beingDragged

    height: content.height
    width: content.width
    visible: shouldBeVisible

    readonly property real implicitXRight: targetComponent.x + targetComponent.width
                                           + QMLStyle.kStandardMargin
    readonly property real implicitXLeft: targetComponent.x - width - QMLStyle.kStandardMargin
    readonly property bool isOnLeft: (targetComponent.x + targetComponent.width + width
                                      + QMLStyle.kStandardMargin) > parent.width
    x: isOnLeft ? implicitXLeft : implicitXRight

    readonly property real implicitY: targetComponent.y + targetComponent.height / 2 - height / 2
    y: clamp(implicitY, 0, parent.height - height)

    property bool mouseIsOnMenu: false

    function clamp(value, minValue, maxValue) {
        return Math.min(Math.max(value, minValue), maxValue)
    }

    onTargetComponentHoveredChanged: {
        hideDelay.start()
    }

    MouseArea {
        id: disableGlobalArea
        hoverEnabled: true
        anchors.fill: content
    }

    Item {
        id: content
        width: childrenRect.width
        height: childrenRect.height

        Row {
            id: gearIndicatorEditRow
            height: true ? implicitHeight : 0
            enabled: true
            LayoutMirroring.enabled: isOnLeft

            ColumnLayout {
                id: gearEditorColumnLeft
                readonly property real maxHeight: Math.max(
                                                      targetComponent.height,
                                                      root.minimumHeight) * 1.5
                height: gearEditorColumnLeft.maxHeight
                spacing: 0
                Slider {
                    id: gearScaleSlider
                    from: root.minScale
                    to: root.maxScale
                    value: targetComponent.scale
                    orientation: Qt.Vertical

                    onValueChanged: {
                        targetComponent.scale = value
                    }

                    Layout.alignment: Qt.AlignHCenter
                    Layout.preferredHeight: gearEditorColumnLeft.maxHeight - resetGearButton.height
                }
                RoundButton {
                    id: resetGearButton
                    contentItem: Item {
                        width: QMLStyle.kStandardTitleIconSize * 0.8
                        height: QMLStyle.kStandardTitleIconSize * 0.8
                        ColoredImage {
                            source: Constants.kIconReset
                            sourceSize.width: parent.width
                            sourceSize.height: parent.height
                            color: parent.parent.checked ? QMLStyle.kAccentColor : QMLStyle.kIconColor
                        }
                    }
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        resetWanted()
                    }
                }
            }

            Item {
                width: 300
                height: gearEditorColumnLeft.height
                ScrollView {
                    id: gearScrollView
                    anchors.fill: parent
                    background: Rectangle {
                        color: QMLStyle.kBackgroundColor
                        anchors.fill: parent
                        radius: QMLStyle.kRadius
                        border.color: QMLStyle.kBorderColor
                        border.width: 1
                    }

                    contentHeight: gearEditorColumn.implicitHeight + QMLStyle.kStandardMargin

                    ColumnLayout {
                        id: gearEditorColumn
                        anchors.fill: parent
                        anchors.margins: Style.kStandardMargin
                        anchors.rightMargin: gearScrollView.verticalScrollWidth
                                             + QMLStyle.kStandardMargin
                    }

                    readonly property real verticalScrollWidth: gearScrollView.ScrollBar.vertical.width
                    readonly property real verticalScrollMax: gearScrollView.ScrollBar.vertical.size

                    readonly property bool isScrolledDown: gearScrollView.ScrollBar.vertical.position + gearScrollView.ScrollBar.vertical.size == 1
                    readonly property bool isScrolledUp: gearScrollView.ScrollBar.vertical.position
                                                         === 0

                    ScrollBar.vertical.policy: ScrollBar.AsNeeded
                }

                ColoredImage {
                    source: Constants.kIconBackArrow
                    rotation: -90
                    color: QMLStyle.kIconColor
                    sourceSize.width: gearScrollView.verticalScrollWidth * 0.6
                    anchors {
                        horizontalCenter: gearScrollView.right
                        horizontalCenterOffset: -gearScrollView.verticalScrollWidth / 2.
                        bottom: gearScrollView.bottom
                        bottomMargin: QMLStyle.kStandardMargin / 2.
                    }
                    visible: !gearScrollView.isScrolledDown
                }

                ColoredImage {
                    source: Constants.kIconBackArrow
                    rotation: 90
                    color: QMLStyle.kIconColor
                    sourceSize.width: gearScrollView.verticalScrollWidth * 0.6
                    anchors {
                        horizontalCenter: gearScrollView.right
                        horizontalCenterOffset: -gearScrollView.verticalScrollWidth / 2.
                        top: gearScrollView.top
                        topMargin: QMLStyle.kStandardMargin / 2.
                    }
                    visible: !gearScrollView.isScrolledUp
                }
            }
        }
    }

    MouseArea {
        id: enterMenuDetectionArea
        hoverEnabled: true
        enabled: !mouseIsOnMenu
        anchors.fill: content
        onEntered: {
            mouseIsOnMenu = true
        }
    }

    Connections {
        target: globalArea
        function onContainsMouseChanged() {
            if (globalArea.containsMouse) {
                mouseIsOnMenu = false
            }
        }
    }

    Timer {
        id: hideDelay
        interval: root.hideDelayMs
        repeat: false
    }
}
