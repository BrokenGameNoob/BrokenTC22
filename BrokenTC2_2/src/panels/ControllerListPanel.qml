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
        text: qsTr("Known controllers list")
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

            required property int index
            required property var modelData

            property bool held: false
            property bool customHovered: (mouseX > 0
                                          && mouseX < grabIcon.x + grabIcon.width
                                          && mouseY > 0
                                          && mouseY < grabIcon.y + grabIcon.height
                                          && containsMouse) || held
            hoverEnabled: true

            anchors {
                left: parent?.left
                right: parent?.right
                leftMargin: -2
                rightMargin: -2
            }
            height: content.height
            width: content.height

            drag.target: held ? content : undefined
            drag.axis: Drag.YAxis

            onPressed: {
                if (customHovered) {
                    held = true
                }
            }
            onReleased: {
                if (!held) {
                    return
                }
                held = false

                var tmp = new Array(visualModel.model.length)
                for (var i = 0; i < tmp.length; i++) {
                    tmp[i] = listView.itemAtIndex(i).modelData
                }

                ServiceManager.controllerHandler.ReorderFromList(tmp)
            }

            Rectangle {
                id: content

                anchors {
                    horizontalCenter: parent.horizontalCenter
                    verticalCenter: parent.verticalCenter
                }
                width: dragArea.width
                height: row.implicitHeight + QMLStyle.kStandardMargin * 2

                border.width: 1
                border.color: dragArea.customHovered ? QMLStyle.kAccentColor : QMLStyle.kBorderColor
                Behavior on border.color {
                    ColorAnimation {
                        duration: 100
                    }
                }

                color: QMLStyle.buttonColor(dragArea.enabled,
                                            dragArea.customHovered,
                                            dragArea.held, true)
                Behavior on color {
                    ColorAnimation {
                        duration: 100
                    }
                }

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

                Row {
                    id: row
                    anchors {
                        fill: parent
                        leftMargin: 2
                        rightMargin: 2
                    }
                    Text {
                        id: indexLabel
                        visible: index > 0
                        font: QMLStyle.kFontH3Bold
                        text: index
                        width: indexLabelMetrics.width + QMLStyle.kStandardMargin * 2
                        anchors.verticalCenter: parent.verticalCenter
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: QMLStyle.kTextColor
                        opacity: 0.3
                        TextMetrics {
                            id: indexLabelMetrics
                            font: indexLabel.font
                            text: indexLabel.text
                        }
                    }
                    ColoredImage {
                        visible: index === 0
                        source: Constants.kIconFavorite
                        sourceSize.width: indexLabel.width * 0.8
                        sourceSize.height: indexLabel.height * 0.8
                        fillMode: Image.PreserveAspectFit
                        width: indexLabel.width
                        color: QMLStyle.kIconColor
                        anchors.verticalCenter: parent.verticalCenter
                        opacity: indexLabel.opacity
                    }

                    Rectangle {
                        color: QMLStyle.kAccentColor
                        opacity: 0.5
                        width: 1
                        height: grabIcon.height
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    ColoredImage {
                        id: grabIcon
                        source: Constants.kIconMoveRowVertical
                        sourceSize.width: Style.kStandardIconWidth
                        sourceSize.height: Style.kStandardIconWidth
                        fillMode: Image.PreserveAspectFit
                        width: Style.kStandardIconWidth + QMLStyle.kStandardMargin
                        color: QMLStyle.kIconColor
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Rectangle {
                        color: QMLStyle.kBorderColor
                        opacity: 0.5
                        width: 1
                        height: grabIcon.height
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    Item {
                        width: QMLStyle.kStandardMargin
                        height: 1
                    }
                    Text {
                        font: QMLStyle.kFontH3
                        text: modelData.Name
                        anchors.verticalCenter: parent.verticalCenter
                        elide: Text.ElideRight
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: QMLStyle.kTextColor
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
        id: listView
        anchors {
            left: parent.left
            right: parent.right
            top: separatorH.bottom
            topMargin: Style.kStandardMargin
            bottom: parent.bottom
        }

        model: visualModel
        clip: true

        cacheBuffer: 10
    }

    property var displayIndexes: new Array(0)
}
