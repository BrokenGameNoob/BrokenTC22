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
    property string selectedOverlayScreen: ServiceManager.settings.SelectedOverlayScreen

    readonly property color modeColor: ServiceManager.gearHandler.isActive ? (ServiceManager.gearHandler.gearMode === GearHandlerMode.CLUTCH_MODE ? overlayModel.ClutchColor : overlayModel.NoClutchColor) : overlayModel.DisabledColor

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

    onSelectedOverlayScreenChanged: {
        refreshScreen()
    }

    function leaveIfEditMode() {
        if (editModeEnabled) {
            leaveEditMode()
        }
    }

    function refreshScreen() {
        console.log("QML refreshScreen to " + root.selectedOverlayScreen)

        for (var i = 0; i < Application.screens.length; i++) {
            if (Application.screens[i].name === root.selectedOverlayScreen) {
                root.screen = Application.screens[i]
                console.log("QML Screen found: " + root.screen.name)
                break
            }
        }
    }

    Shortcut {
        sequence: "Escape"
        onActivated: {
            console.log("ESC pressed")
            leaveIfEditMode()
        }
    }

    SoftRunningIcon {
        id: btc2LaunchedIcon
        anchors {
            bottom: parent.bottom
            left: parent.left
            margins: QMLStyle.kStandardMargin
            bottomMargin: 50
        }
    }

    Item {
        id: content
        anchors.fill: parent
        visible: ServiceManager.focusedGame != Game.NONE || root.editModeEnabled
                 || ServiceManager.settings.OverlayAlwaysVisible

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

        Item {
            id: gearLabel

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height - QMLStyle.kStandardMargin - height

            readonly property real implicitX: overlayModel.GearX
            readonly property real implicitY: overlayModel.GearY

            x: implicitX === 0 ? defaultX : implicitX
            y: implicitY === 0 ? defaultY : implicitY

            height: gearLabelContent.implicitHeight
            width: gearLabelContent.implicitWidth

            Rectangle {
                color: overlayModel.GearBackgroundColor
                radius: width > height ? height / 2 : width / 2
                opacity: 1
                anchors {
                    fill: parent
                }
            }

            RowLayout {
                id: gearLabelContent
                anchors.centerIn: parent
                spacing: 0
                clip: true

                Label {
                    id: gearLabelActualText
                    font.pointSize: 50
                    font.bold: true

                    Layout.preferredWidth: implicitWidth + QMLStyle.kStandardMargin
                    padding: Style.kStandardMargin / 2
                    topPadding: Style.kStandardMargin / 2 - (0.12 * font.pixelSize)
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    visible: overlayModel.GearEnabled || editModeEnabled

                    text: ServiceManager.gearHandler.gearStr
                    color: overlayModel.GearChangeGearColorDependingOnMode ? modeColor : QMLStyle.kTextColor
                }

                Text {
                    id: gearLabelDisabledImage
                    readonly property string implicitText: ServiceManager.gearHandler.gearModeStr.toUpperCase()
                    Layout.alignment: Qt.AlignVCenter

                    text: implicitText
                    color: "white"
                    font: QMLStyle.kFontH2Bold
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft

                    Layout.preferredWidth: text.length > 0
                                           && overlayModel.GearIndicatorModeNotifEnabled ? implicitWidth + QMLStyle.kStandardMargin * 2 : 0
                    visible: Layout.preferredWidth > 0

                    Behavior on Layout.preferredWidth {
                        NumberAnimation {
                            duration: 150
                            easing.type: Easing.InOutQuad
                        }
                    }

                    onImplicitTextChanged: {
                        gearLabelDisabledImage.text = implicitText
                        resetGearLabelDisabledImageText.restart()
                    }
                    Timer {
                        id: resetGearLabelDisabledImageText
                        interval: 1000
                        running: false
                        repeat: false
                        onTriggered: {
                            gearLabelDisabledImage.text = ""
                        }
                    }
                }
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

        Label {
            id: debugLabel

            text: editModeEnabled ? qsTr("DEBUG text") : (ServiceManager.settings.SelectedOverlayScreen)

            readonly property real defaultX: QMLStyle.kStandardMargin
            readonly property real defaultY: parent.height / 2. - height / 2.

            readonly property real implicitX: overlayModel.DebugLabelX
            readonly property real implicitY: overlayModel.DebugLabelY

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

            readonly property bool shouldBeVisible: true
            visible: (overlayModel.DebugLabelEnabled && width > 0)
                     || editModeEnabled

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
                id: dragdebugLabel
                anchors.fill: parent
                target: parent
                minScale: root.minScale
                maxScale: root.maxScale
                onDropped: {
                    overlayModel.DebugLabelX = debugLabel.x
                    overlayModel.DebugLabelY = debugLabel.y
                }
            }

            property bool inhibitScaleUpdate: false
            Binding {
                target: debugLabel
                property: "scale"
                value: overlayModel.DebugLabelScaling
            }
            onScaleChanged: {
                inhibitScaleUpdate = true
                overlayModel.DebugLabelScaling = scale
                inhibitScaleUpdate = false
            }
        }

        GameOverlayEditMenu {
            globalArea: globalArea
            editModeEnabled: root.editModeEnabled
            targetEnableDragForComponent: dragdebugLabel
            targetComponent: debugLabel
            minScale: root.minScale
            maxScale: root.maxScale

            GroupedEditor {
                id: debugEditor
                targetElement: root.overlayModel
                targetGroup: "debug"
                title: qsTr("Debug")
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            onResetWanted: {
                confirmationPopup.openForElement(
                            debugEditor.title,
                            debugEditor.resetAllFieldsForGroupTitle)
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
                    source: ServiceManager.gearHandler.isActive ? ServiceManager.gearHandler.gearModeIconSource : Constants.kIconCancelInverted
                    color: overlayModel.ModeIndicatorChangeGearColorDependingOnMode ? modeColor : QMLStyle.kTextColor
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
                        resetClutchModeTextTimer.restart()
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
