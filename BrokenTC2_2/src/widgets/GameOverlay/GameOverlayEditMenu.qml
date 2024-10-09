import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../../utils"
// import "../../utils/utils.js" as Utils
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

    anchors {
        verticalCenter: targetComponent.verticalCenter
        leftMargin: QMLStyle.kStandardMargin
    }
    readonly property real implicitXRight: targetComponent.x + targetComponent.width
                                           + QMLStyle.kStandardMargin
    readonly property real implicitXLeft: targetComponent.x - width - QMLStyle.kStandardMargin
    x: (targetComponent.x + targetComponent.width + width
        + QMLStyle.kStandardMargin) > parent.width ? implicitXLeft : implicitXRight

    property bool mouseIsOnMenu: false

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
                        console.error("DO ME")
                        //ask confirmation to the user before committing
                        resetWanted()
                    }
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
