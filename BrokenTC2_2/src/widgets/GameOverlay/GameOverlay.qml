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
                confirmationPopup.openForAll()
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

        function resetAll() {
            gearEditor.resetAllFieldsForGroupTitle()
            notifEditor.resetAllFieldsForGroupTitle()
            clutchModeEditor.resetAllFieldsForGroupTitle()
        }

        Label {
            id: gearLabel

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height - QMLStyle.kStandardMargin - height

            readonly property real implicitX: overlayModel.GearX
            readonly property real implicitY: overlayModel.GearY

            x: implicitX === 0 ? defaultX : implicitX
            y: implicitY === 0 ? defaultY : implicitY

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
                color: overlayModel.GearBackgroundColor
                radius: width > height ? height / 2 : width / 2
                opacity: 1
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
            targetEnableDragForComponent: dragGearLabel
            targetComponent: gearLabel
            minScale: root.minScale
            maxScale: root.maxScale

            GroupedEditor {
                id: gearEditor
                targetElement: root.overlayModel
                targetGroup: "gear"
                title: qsTr("Gear indicator")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            onResetWanted: {
                confirmationPopup.openForElement(
                            gearEditor.title,
                            gearEditor.resetAllFieldsForGroupTitle)
            }
        }

        Label {
            id: notifLabel

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height / 2. - height / 2.

            readonly property real implicitX: overlayModel.NotifX
            readonly property real implicitY: overlayModel.NotifY

            x: implicitX === 0 ? defaultX : implicitX
            y: implicitY === 0 ? defaultY : implicitY

            font.pointSize: 30
            font.bold: false

            height: implicitHeight
            width: shouldBeVisible
                   || editModeEnabled ? implicitWidth + QMLStyle.kStandardMargin : 0

            Behavior on width {
                SmoothedAnimation {
                    duration: 150
                }
            }

            padding: Style.kStandardMargin
            topPadding: Style.kStandardMargin / 2
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            clip: true

            readonly property bool shouldBeVisible: ServiceManager.overlayNotificationText.length > 0
            visible: (overlayModel.NotifEnabled && width > 0) || editModeEnabled

            text: editModeEnabled ? qsTr("Notification text") : ServiceManager.overlayNotificationText

            background: Item {
                Rectangle {
                    anchors.fill: parent
                    color: overlayModel.NotifBackgroundColor
                    radius: width > height ? height / 2 : width / 2
                    opacity: 1.
                }

                Rectangle {
                    visible: editModeEnabled
                    color: QMLStyle.kAccentColor
                    width: 3
                    opacity: 1.
                    anchors {
                        horizontalCenter: parent.left
                        top: parent.top
                        topMargin: -QMLStyle.kStandardMargin / 2.
                        bottom: parent.bottom
                        bottomMargin: -QMLStyle.kStandardMargin / 2.
                    }
                }
            }

            EnableDragForComponent {
                id: dragNotifLabel
                anchors.fill: parent
                target: parent
                minScale: root.minScale
                maxScale: root.maxScale
                onDropped: {
                    overlayModel.NotifX = notifLabel.x
                    overlayModel.NotifY = notifLabel.y
                }
            }

            property bool inhibitScaleUpdate: false
            Binding {
                target: notifLabel
                property: "scale"
                value: overlayModel.NotifScaling
            }
            onScaleChanged: {
                inhibitScaleUpdate = true
                overlayModel.NotifScaling = scale
                inhibitScaleUpdate = false
            }
        }

        GameOverlayEditMenu {
            globalArea: globalArea
            editModeEnabled: root.editModeEnabled
            targetEnableDragForComponent: dragNotifLabel
            targetComponent: notifLabel
            minScale: root.minScale
            maxScale: root.maxScale

            GroupedEditor {
                id: notifEditor
                targetElement: root.overlayModel
                targetGroup: "notif"
                title: qsTr("Notifications")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            onResetWanted: {
                confirmationPopup.openForElement(
                            notifEditor.title,
                            notifEditor.resetAllFieldsForGroupTitle)
            }
        }

        Item {
            id: clutchModeLabel

            readonly property real defaultX: QMLStyle.kStandardMargin + 200 //TODO:Adjust
            readonly property real defaultY: parent.height - QMLStyle.kStandardMargin - height

            readonly property real implicitX: overlayModel.ModeIndicatorX
            readonly property real implicitY: overlayModel.ModeIndicatorY

            x: implicitX === 0 ? defaultX : implicitX
            y: implicitY === 0 ? defaultY : implicitY

            visible: overlayModel.ModeIndicatorEnabled || editModeEnabled

            width: clutchModeLabelContent.width + QMLStyle.kStandardMargin
            height: clutchModeLabelContent.height + QMLStyle.kStandardMargin

            Rectangle {
                anchors.fill: parent
                color: overlayModel.GearBackgroundColor
                radius: width > height ? height / 2 : width / 2
                opacity: 1
            }

            Row {
                id: clutchModeLabelContent
                anchors.centerIn: parent
                spacing: 0
                ColoredImage {
                    sourceSize.width: (gearLabel.height - QMLStyle.kStandardMargin) * scale
                    sourceSize.height: (gearLabel.height - QMLStyle.kStandardMargin) * scale
                    source: ServiceManager.gearHandler.gearModeIconSource
                    color: ServiceManager.gearHandler.gearMode
                           === GearHandlerMode.CLUTCH_MODE ? overlayModel.ClutchColor : overlayModel.NoClutchColor
                }
                Text {
                    id: clutchModeText
                    readonly property string implicitText: ServiceManager.gearHandler.gearModeStr.toUpperCase()
                    anchors.verticalCenter: parent.verticalCenter
                    text: implicitText
                    color: "white"
                    font: QMLStyle.kFontH2Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter

                    width: text.length > 0 ? implicitWidth : 0

                    Behavior on width {
                        SmoothedAnimation {
                            duration: 150
                        }
                    }

                    onImplicitTextChanged: {
                        clutchModeText.text = implicitText
                        resetClutchModeTextTimer.start()
                    }
                    Timer {
                        id: resetClutchModeTextTimer
                        interval: 1500
                        running: false
                        repeat: false
                        onTriggered: {
                            clutchModeText.text = ""
                        }
                    }
                }
            }

            EnableDragForComponent {
                id: dragClutchModeLabel
                anchors.fill: parent
                target: parent
                minScale: root.minScale
                maxScale: root.maxScale
                onDropped: {
                    overlayModel.ModeIndicatorX = clutchModeLabel.x
                    overlayModel.ModeIndicatorY = clutchModeLabel.y
                }
            }

            property bool inhibitScaleUpdate: false
            Binding {
                target: clutchModeLabel
                property: "scale"
                value: overlayModel.ModeIndicatorScaling
            }
            onScaleChanged: {
                inhibitScaleUpdate = true
                overlayModel.ModeIndicatorScaling = scale
                inhibitScaleUpdate = false
            }
        }
        GameOverlayEditMenu {
            globalArea: globalArea
            editModeEnabled: root.editModeEnabled
            targetEnableDragForComponent: dragClutchModeLabel
            targetComponent: clutchModeLabel
            minScale: root.minScale
            maxScale: root.maxScale

            GroupedEditor {
                id: clutchModeEditor
                targetElement: root.overlayModel
                targetGroup: "mode"
                title: qsTr("Clutch mode")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            onResetWanted: {
                confirmationPopup.openForElement(
                            clutchModeEditor.title,
                            clutchModeEditor.resetAllFieldsForGroupTitle)
            }
        }
    }

    ConfirmationDialog {
        id: confirmationPopup
        function openForElement(elementTitle, callback) {
            text = qsTr("Are you sure you want to reset") + " " + elementTitle + qsTr(
                        "?")
            confirmationPopup.confirm(callback)
        }
        function openForAll() {
            text = qsTr("Are you sure you want to reset all elements?")
            confirmationPopup.confirm(content.resetAll)
        }
    }
}
