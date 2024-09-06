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
    clip: false

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

        spacing: Style.kStandardMargin

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
                            return buttonComponent
                        case DataEditor.SLIDER:
                            return sliderComponent
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
                        topMargin: height / 2 + col.spacing / 2 + 1
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
            color: "blue"
            anchors.centerIn: parent
        }
    }

    Component {
        id: textDisplayComponent
        Text {
            text: customValue
            font: QMLStyle.kFontH4Bold
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            color: Style.kForeground
            anchors.centerIn: parent
            height: implicitHeight + Style.kStandardMargin / 2

            Rectangle {
                color: "transparent"
                border.color: Qt.darker(Style.kLightGrey, 1.5)
                border.width: 1
                anchors.fill: parent
                radius: parent.height / 2
            }
        }
    }

    Component {
        id: buttonComponent
        Button {
            text: qsTr("Button: ") + customValue
            font: QMLStyle.kFontH4Bold
            anchors.centerIn: parent
            height: implicitHeight * 0.8
        }
    }

    Component {
        id: sliderComponent
        Slider {
            anchors.centerIn: parent
            height: implicitHeight * 0.8
            value: customValue
            onPressedChanged: {
                if (!pressed) {
                    setValue(value)
                }
            }
        }
    }
}
