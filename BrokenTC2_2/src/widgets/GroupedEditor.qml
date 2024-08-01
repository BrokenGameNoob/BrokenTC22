import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Layouts

import btc2

Item {
    required property var targetElement
    required property string targetGroup

    property alias title: mainGroupBox.title

    implicitHeight: mainGroupBox.implicitHeight

    GroupBox {
        id: mainGroupBox
        anchors.fill: parent

        ColumnLayout {
            id: col
            anchors.fill: parent

            Repeater {
                model: targetElement.GetPropertiesKeysFor(targetGroup)

                delegate: Item {
                    id: delegateItem

                    Layout.fillWidth: true

                    height: editorTitle.implicitHeight

                    property string key: modelData
                    property var value: targetElement[key]
                    property var title: targetElement.GetTitleFor(key)

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

                    Rectangle {
                        color: Style.kAccent
                        opacity: 0.5
                        anchors {
                            verticalCenter: editorTitle.verticalCenter
                            left: editorTitle.right
                            right: parent.horizontalCenter
                        }
                        height: 1
                    }

                    Text {
                        anchors {
                            left: parent.horizontalCenter
                            // right: parent.right
                            bottom: parent.bottom
                            top: parent.top
                        }

                        text: value
                        color: Style.kForeground
                    }
                }
            }
        }
    }

    Component {
        id: textInputComponent
        TextInput {
            // Define properties and behavior for TextInput
            property var value
            text: value
        }
    }

    Component {
        id: spinBoxComponent
        SpinBox { // Define properties and behavior for SpinBox
        }
    }
}
