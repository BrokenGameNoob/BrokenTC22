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

    readonly property real implicitHeight2: {
        col.implicitHeight + titleText.implicitHeight + Style.kStandardMargin
    }
    implicitHeight: implicitHeight2

    Text {
        id: titleText
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: Style.kStandardMargin * 2
        }
        font: QMLStyle.kFontH2
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

        spacing: Style.kStandardMargin

        Repeater {
            id: mainRepeater
            model: targetElement.GetPropertiesKeysFor(targetGroup)

            delegate: Item {
                id: delegateItem

                Layout.fillWidth: true

                height: editorTitle.implicitHeight

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
                        bottom: parent.bottom
                        top: parent.top
                    }

                    text: title
                    color: Style.kForeground
                }

                Loader {
                    id: editorLoader
                    anchors {
                        left: parent.horizontalCenter
                        bottom: parent.bottom
                        top: parent.top
                    }
                    sourceComponent: {
                        switch (targetElement.GetEditorTypeFor(key)) {
                        case "TextInput":
                            return textInputComponent
                        case "SpinBox":
                            return spinBoxComponent
                        case "Button":
                            return buttonComponent
                        default:
                            return null
                        }
                    }
                    onLoaded: {
                        item.value = value
                    }
                }

                Rectangle {
                    color: Style.kLightGrey
                    opacity: 0.35
                    anchors {
                        top: parent.bottom
                        left: parent.left
                        right: parent.right
                        topMargin: height / 2 + col.spacing / 2 + 1
                    }
                    height: 1
                    visible: !isLastElement
                }
            }
        }
    }

    Component {
        id: textInputComponent
        TextInput {
            property var value
            text: value
            onTextChanged: value = text
        }
    }

    Component {
        id: spinBoxComponent
        SpinBox {
            property var value
            value: value
            onValueChanged: value = value
        }
    }

    Component {
        id: buttonComponent
        Button {
            property var value
            text: "Button"
            onClicked: console.log("Button clicked")
        }
    }
}
