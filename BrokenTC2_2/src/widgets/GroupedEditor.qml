import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../utils"
import "../widgets"

import btc2

Item {
    id: frame
    required property var targetElement
    required property string targetGroup

    property alias title: titleText.text
    property bool alignTitleLeft: true
    clip: false

    readonly property real componentHeight: 35

    readonly property real implicitHeight2: {
        col.implicitHeight + titleText.implicitHeight + Style.kStandardMargin
    }
    implicitHeight: implicitHeight2

    /* WARNING: this will reset all the fields for the current group key. Not only the visible ones */
    function resetAllFieldsForGroupTitle() {
        targetElement.Reset(targetGroup)
    }

    Text {
        id: titleText
        anchors {
            top: parent.top
            left: alignTitleLeft ? parent.left : undefined
            right: alignTitleLeft ? undefined : parent.right
            leftMargin: alignTitleLeft ? Style.kStandardMargin * 2 : 0
            rightMargin: alignTitleLeft ? 0 : Style.kStandardMargin * 2
        }
        font: QMLStyle.kFontH3Bold
        color: "white"
    }

    ColumnLayout {
        id: col
        anchors {
            top: titleText.bottom
            topMargin: Style.kStandardMargin
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        spacing: 0

        Repeater {
            id: mainRepeater
            model: targetElement.GetPropertiesKeysFor(targetGroup)

            delegate: Item {
                id: delegateItem

                Layout.fillWidth: true
                height: editorLoader.height

                property string key: modelData
                property var value: targetElement[key]
                property var title: targetElement.GetTitleFor(key)
                property bool isLastElement: model.index === mainRepeater.count - 1

                visible: targetElement.IsKeyCompatibleWithGame(
                             key, ServiceManager.gameSelector.selectedGame)

                Text {
                    id: editorTitle
                    anchors {
                        left: parent.left
                        verticalCenter: parent.verticalCenter
                    }

                    text: title
                    color: Style.kForeground
                    font: QMLStyle.kFontH4
                }

                Loader {
                    id: editorLoader

                    property string key: delegateItem.key
                    property var model: targetElement
                    property var customValue: model[key]

                    function setValue(value) {
                        model[key] = value
                    }

                    anchors {
                        verticalCenter: parent.verticalCenter
                        right: parent.right
                        left: parent.horizontalCenter
                    }
                    // height: childrenRect.height
                    height: (item !== null
                             && typeof (item) !== 'undefined') ? item.height : 0

                    sourceComponent: {
                        let type = targetElement.GetEditorTypeFor(key)

                        switch (type) {
                        case DataEditor.RAW_DISPLAY:
                            return textDisplayComponent
                        case DataEditor.CONTROLLER_KEY:
                            return controllerInputComponent
                        case DataEditor.KEYBOARD_KEY:
                            return keyboardInputComponent
                        case DataEditor.SLIDER:
                            return sliderComponent
                        case DataEditor.SWITCH:
                            return switchComponent
                        case DataEditor.COLOR:
                            return colorComponent
                        }
                        return unknownComponent
                    }
                }

                Rectangle {
                    color: Style.kLightGrey
                    opacity: 0.1
                    anchors {
                        top: parent.bottom
                        left: parent.left
                        right: parent.right
                        topMargin: height / 2 + col.spacing / 2
                    }
                    height: 1
                    visible: !isLastElement
                }
            }
        }
    }

    Component {
        id: unknownComponent
        Rectangle {
            id: unknownComponentChild
            color: "blue"
            anchors.centerIn: parent
            height: componentHeight
        }
    }

    Component {
        id: textDisplayComponent
        Text {
            id: textDisplayComponentChild
            text: customValue
            font: QMLStyle.kFontH4Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Style.kForeground
            anchors.centerIn: parent
            height: componentHeight

            elide: Text.ElideRight
            leftPadding: QMLStyle.kStandardMargin
            rightPadding: QMLStyle.kStandardMargin

            Rectangle {
                color: "transparent"
                border.color: Qt.darker(Style.kLightGrey, 1.5)
                border.width: 1

                height: parent.implicitHeight + QMLStyle.kStandardMargin
                anchors {
                    left: parent.left
                    right: parent.right
                    verticalCenter: parent.verticalCenter
                }

                radius: parent.height / 2
            }
        }
    }

    Component {
        id: controllerInputComponent
        Button {
            id: controllerButton
            text: customValue < 0 ? qsTr("NOT SET") : customValue
            font: QMLStyle.kFontH4Bold
            anchors.centerIn: parent
            height: componentHeight

            property bool isButtonDown: false

            onClicked: {
                controllerBindingPopup.openForButton(this)
            }

            function setFromPopup(button) {
                setValue(button)
            }

            Rectangle {
                height: controllerButton.height / 3
                width: height
                radius: height / 2
                color: QMLStyle.kPrimaryColor
                visible: controllerButton.isButtonDown

                anchors {
                    verticalCenter: controllerButton.verticalCenter
                    right: controllerButton.right
                    rightMargin: QMLStyle.kStandardMargin
                }
            }

            Connections {
                target: ServiceManager.controllerHandler
                function onButtonDown(button) {
                    if (button !== customValue || isButtonDown) {
                        return
                    }
                    isButtonDown = true
                }
                function onButtonUp(button) {
                    if (button !== customValue || !isButtonDown) {
                        return
                    }
                    isButtonDown = false
                    setValue(button)
                }
            }
        }
    }

    Component {
        id: keyboardInputComponent
        Button {
            id: keyboardButton
            readonly property string keyNameFromFunction: ServiceManager.keyboardHandler.GetKeyName(
                                                              customValue)

            text: customValue < 0 ? qsTr("NOT SET") : (keyNameFromFunction ? keyNameFromFunction : "<?-" + customValue + "-?>")
            font: QMLStyle.kFontH4Bold
            anchors.centerIn: parent
            height: componentHeight

            property bool isButtonDown: false

            onClicked: {
                keyboardBindingPopup.openForButton(this)
            }

            function setFromPopup(button) {
                setValue(button)
            }

            Rectangle {
                height: keyboardButton.height / 3
                width: height
                radius: height / 2
                color: QMLStyle.kPrimaryColor
                visible: keyboardButton.isButtonDown

                anchors {
                    verticalCenter: keyboardButton.verticalCenter
                    right: keyboardButton.right
                    rightMargin: QMLStyle.kStandardMargin
                }
            }

            Connections {
                target: ServiceManager.keyboardHandler
                function onKeyDown(key) {
                    if (key !== customValue || isButtonDown) {
                        return
                    }
                    isButtonDown = true
                }
                function onKeyUp(key) {
                    if (key !== customValue || !isButtonDown) {
                        return
                    }
                    isButtonDown = false
                    setValue(key)
                }
            }
        }
    }

    Component {
        id: sliderComponent
        Slider {
            id: sliderComponentChild
            anchors.centerIn: parent
            height: componentHeight
            value: customValue
            onPressedChanged: {
                if (!pressed) {
                    setValue(value)
                }
            }
        }
    }

    Component {
        id: switchComponent
        Rectangle {
            width: parent.width
            height: componentHeight
            color: "transparent"

            readonly property real wantedHeight: componentHeight * 0.7

            Switch {
                id: control
                anchors.centerIn: parent
                checked: customValue
                onCheckedChanged: {
                    setValue(checked)
                }
                scale: wantedHeight / height
            }
        }
    }

    Component {
        id: colorComponent
        Button {
            height: componentHeight
            contentItem: Rectangle {
                color: customValue
                width: parent.width * 0.9
                height: parent.height * 0.9
                anchors.centerIn: parent
                radius: height / 2.
            }

            onClicked: {
                colorPickPopup.selectColor(customValue, this.onColorSelected)
            }

            function onColorSelected(color) {
                setValue(color)
            }
        }
    }

    ColorPicker {
        id: colorPickPopup

        // title: qsTr("Select color")
        // parentWindow: frame.parentWindow
    }

    ThemedPopup {
        id: controllerBindingPopup
        width: Math.min(Overlay.overlay.width / 2., 600)
        height: Math.min(Overlay.overlay.height / 2., 300)
        anchors.centerIn: Overlay.overlay
        modal: true

        borderColor: QMLStyle.kAccentColor
        borderWidth: 1

        property var linkedButton: null

        function openForButton(button) {
            linkedButton = button
            controllerBindingPopup.open()
        }

        Item {
            Connections {
                enabled: ServiceManager.controllerHandler.isInEnterKeybindMode
                         && controllerBindingPopup.visible
                target: ServiceManager.controllerHandler
                function onButtonDown(button) {
                    console.log("GOT BUTTON: " + button)
                    if (controllerBindingPopup.linkedButton !== null) {
                        controllerBindingPopup.linkedButton.setFromPopup(button)
                    }
                    controllerBindingPopup.close()
                }
            }

            anchors.fill: parent

            ColoredImage {
                source: Constants.kIconController
                anchors {
                    top: parent.top
                    topMargin: Style.kStandardMargin
                    right: parent.right
                    rightMargin: Style.kStandardMargin
                }
                sourceSize.width: QMLStyle.kStandardTitleIconSize
                sourceSize.height: QMLStyle.kStandardTitleIconSize
                color: QMLStyle.kTextColor
            }

            RoundButton {
                id: cancelControllerBindingPopup
                anchors {
                    left: parent.left
                    leftMargin: Style.kStandardMargin
                    top: parent.top
                    topMargin: Style.kStandardMargin
                }

                height: QMLStyle.kStandardTitleIconSize * 1.2
                width: height

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
                    controllerBindingPopup.close()
                }

                ToolTip.text: qsTr("Cancel")
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }

            RoundButton {
                anchors {
                    left: parent.left
                    leftMargin: Style.kStandardMargin
                    bottom: parent.bottom
                    bottomMargin: Style.kStandardMargin
                }

                height: cancelControllerBindingPopup.height
                width: height

                contentItem: Item {
                    width: parent.width * 0.8
                    height: parent.height * 0.8
                    ColoredImage {
                        source: Constants.kIconDelete
                        sourceSize.width: parent.width
                        sourceSize.height: parent.height
                        color: QMLStyle.kErrorRed
                    }
                }

                onClicked: {
                    controllerBindingPopup.close()
                    if (controllerBindingPopup.linkedButton !== null) {
                        controllerBindingPopup.linkedButton.setFromPopup(-1)
                    }
                }

                ToolTip.text: qsTr("Delete binding")
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }

            LoadingIcon {
                id: loadingIcon
                backgroundColor: QMLStyle.kAccentColor
                foregroundColor: QMLStyle.kPrimaryColor
                width: QMLStyle.kStandardTitleIconSize
                height: width
                anchors {
                    top: parent.verticalCenter
                    topMargin: QMLStyle.kStandardMargin / 2
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: qsTr("Waiting for controller input")
                font: QMLStyle.kFontH3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: Style.kForeground
                anchors {
                    bottom: parent.verticalCenter
                    bottomMargin: QMLStyle.kStandardMargin / 2
                    left: parent.left
                    right: parent.right
                }
            }
        }

        onVisibleChanged: {
            if (visible) {
                ServiceManager.controllerHandler.EnterKeybindMode()
            } else {
                ServiceManager.controllerHandler.LeaveKeybindMode()
            }
        }
    }

    ThemedPopup {
        id: keyboardBindingPopup
        width: Math.min(Overlay.overlay.width / 2., 600)
        height: Math.min(Overlay.overlay.height / 2., 300)
        anchors.centerIn: Overlay.overlay
        modal: true

        borderColor: QMLStyle.kAccentColor
        borderWidth: 1

        property var linkedButton: null

        function openForButton(button) {
            linkedButton = button
            keyboardBindingPopup.open()
        }

        Item {
            Connections {
                enabled: keyboardBindingPopup.visible
                         && ServiceManager.keyboardHandler.isInEnterKeybindMode
                // enabled: true
                target: ServiceManager.keyboardHandler
                function onKeyDown(key) {
                    if (keyboardBindingPopup.linkedButton !== null) {
                        keyboardBindingPopup.linkedButton.setFromPopup(key)
                    }
                    keyboardBindingPopup.close()
                }
            }

            anchors.fill: parent

            ColoredImage {
                source: Constants.kIconKeyboard
                anchors {
                    top: parent.top
                    topMargin: Style.kStandardMargin
                    right: parent.right
                    rightMargin: Style.kStandardMargin
                }
                sourceSize.width: QMLStyle.kStandardTitleIconSize
                sourceSize.height: QMLStyle.kStandardTitleIconSize
                color: QMLStyle.kTextColor
            }

            RoundButton {
                id: cancelkeyboardBindingPopup
                anchors {
                    left: parent.left
                    leftMargin: Style.kStandardMargin
                    top: parent.top
                    topMargin: Style.kStandardMargin
                }

                height: QMLStyle.kStandardTitleIconSize * 1.2
                width: height

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
                    keyboardBindingPopup.close()
                }

                ToolTip.text: qsTr("Cancel")
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }

            RoundButton {
                anchors {
                    left: parent.left
                    leftMargin: Style.kStandardMargin
                    bottom: parent.bottom
                    bottomMargin: Style.kStandardMargin
                }

                height: cancelkeyboardBindingPopup.height
                width: height

                contentItem: Item {
                    width: parent.width * 0.8
                    height: parent.height * 0.8
                    ColoredImage {
                        source: Constants.kIconDelete
                        sourceSize.width: parent.width
                        sourceSize.height: parent.height
                        color: QMLStyle.kErrorRed
                    }
                }

                onClicked: {
                    keyboardBindingPopup.close()
                    if (keyboardBindingPopup.linkedButton !== null) {
                        keyboardBindingPopup.linkedButton.setFromPopup(-1)
                    }
                }

                ToolTip.text: qsTr("Delete binding")
                ToolTip.visible: hovered
                ToolTip.delay: 1000
            }

            LoadingIcon {
                backgroundColor: QMLStyle.kAccentColor
                foregroundColor: QMLStyle.kPrimaryColor
                width: QMLStyle.kStandardTitleIconSize
                height: width
                anchors {
                    top: parent.verticalCenter
                    topMargin: QMLStyle.kStandardMargin / 2
                    horizontalCenter: parent.horizontalCenter
                }
            }

            Text {
                text: qsTr("Waiting for keyboard input")
                font: QMLStyle.kFontH3
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: Style.kForeground
                anchors {
                    bottom: parent.verticalCenter
                    bottomMargin: QMLStyle.kStandardMargin / 2
                    left: parent.left
                    right: parent.right
                }
            }
        }

        onVisibleChanged: {
            if (visible) {
                ServiceManager.keyboardHandler.EnterKeybindMode()
            } else {
                ServiceManager.keyboardHandler.LeaveKeybindMode()
            }
        }
    }
}
