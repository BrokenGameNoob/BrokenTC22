import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import ".."
import "../utils"
import "../widgets"

import btc2

ThemedPopup {
    id: confirmationPopup
    width: Math.min(Overlay.overlay.width / 2., 600)
    height: Math.min(Overlay.overlay.height / 2., 300)
    anchors.centerIn: Overlay.overlay
    modal: true

    property string title: qsTr("Confirmation needed")
    property string yesButtonText: qsTr("Yes")
    property string cancelButtonText: qsTr("Cancel")

    property alias label: mainContentText

    borderColor: QMLStyle.kAccentColor
    borderWidth: 1

    property string text: ""
    property var callback: null

    function confirm(_callback) {
        confirmationPopup.callback = _callback
        confirmationPopup.open()
    }

    Item {
        anchors.fill: parent
        Label {
            id: confirmationPopupTitle
            font: QMLStyle.kFontH2Bold
            text: confirmationPopup.title
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
                margins: QMLStyle.kStandardMargin
            }
        }

        Rectangle {
            height: 1
            anchors {
                verticalCenter: confirmationPopupTitle.bottom
                verticalCenterOffset: QMLStyle.kStandardMargin
                left: parent.left
                right: parent.right
            }
            color: QMLStyle.kBorderColor
            opacity: 0.5
        }

        Label {
            id: mainContentText
            font: QMLStyle.kFontH3
            text: confirmationPopup.text
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter

            anchors {
                top: confirmationPopupTitle.bottom
                left: parent.left
                right: parent.right
                margins: QMLStyle.kStandardMargin
                topMargin: QMLStyle.kStandardMargin * 2
                bottom: confirmationPopupBottomRow.top
            }
        }

        RowLayout {
            id: confirmationPopupBottomRow
            anchors {
                bottom: parent.bottom
                left: parent.left
                right: parent.right
                margins: QMLStyle.kStandardMargin
            }

            Item {
                Layout.fillWidth: true
            }
            Button {
                text: confirmationPopup.yesButtonText

                icon.source: Constants.kIconOk
                icon.color: QMLStyle.kOkColor

                onClicked: {
                    if (confirmationPopup.callback) {
                        confirmationPopup.callback()
                    }
                    confirmationPopup.close()
                }
            }
            Item {
                Layout.preferredWidth: QMLStyle.kStandardMargin
            }
            Button {
                text: confirmationPopup.cancelButtonText

                icon.source: Constants.kIconCancel
                icon.color: QMLStyle.kCancelColor
                onClicked: {
                    confirmationPopup.close()
                }
            }
        }
    }
}
