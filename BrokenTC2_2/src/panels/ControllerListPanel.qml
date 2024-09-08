import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15
import "../tabs"
import "../debug"
import "../utils"
import "../widgets"
import ".."
import "."
pragma ComponentBehavior

import btc2

Item {
    id: root
    signal leaveControllerListPanel

    RoundButton {
        id: controllerListLeaveButton
        anchors {
            left: parent.left
            leftMargin: Style.kStandardMargin
            top: titleButton.top
            topMargin: Style.kStandardMargin / 2
            bottom: titleButton.bottom
            bottomMargin: Style.kStandardMargin / 2
        }

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
            leaveControllerListPanel()
        }
    }

    TabButton {
        id: titleButton
        text: qsTr("Controllers list")
        icon.source: Constants.kIconControllerList
        icon.width: Style.kStandardIconWidth
        icon.height: Style.kStandardIconWidth

        checkable: false

        anchors {
            top: parent.top
            horizontalCenter: parent.horizontalCenter
        }

        onClicked: {
            leaveControllerListPanel()
        }
    }

    Rectangle {
        id: separatorH
        anchors {
            top: titleButton.bottom
            left: parent.left
            right: parent.right
        }
        color: QMLStyle.kBorderColor
        opacity: 0.5
        height: 1
    }

    Component {
        id: dragDelegate

        MouseArea {
            id: dragArea

            property bool held: false

            anchors {
                left: parent?.left
                right: parent?.right
            }
            height: content.height

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressAndHold: held = true
            onReleased: held = false

            Rectangle {
                id: content

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width
                height: column.implicitHeight + 4

                border.width: 1
                border.color: "lightsteelblue"

                color: dragArea.held ? "lightsteelblue" : "white"
                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }

                radius: 2

                Drag.active: dragArea.held
                Drag.source: dragArea
                Drag.hotSpot.x: width / 2
                Drag.hotSpot.y: height / 2

                states: State {
                    when: dragArea.held

                    ParentChange {
                        target: content
                        parent: root
                    }
                    AnchorChanges {
                        target: content
                        anchors {
                            horizontalCenter: undefined
                            verticalCenter: undefined
                        }
                    }
                }

                Column {
                    id: column
                    anchors {
                        fill: parent
                        margins: 2
                    }

                    Text {
                        text: qsTr('Name: ') + modelData.Name
                    }
                }
            }

            DropArea {
                anchors {
                    fill: parent
                }

                onEntered: drag => {
                               visualModel.items.move(
                                   drag.source.DelegateModel.itemsIndex,
                                   dragArea.DelegateModel.itemsIndex)
                           }
            }
        }
    }

    DelegateModel {
        id: visualModel

        model: ServiceManager.controllerHandler.knownControllersProfiles
        delegate: dragDelegate
    }

    ListView {
        anchors {
            left: parent.left
            right: parent.right
            top: separatorH.bottom
            topMargin: Style.kStandardMargin
            bottom: parent.bottom
        }

        model: visualModel

        cacheBuffer: 10
    }
}
