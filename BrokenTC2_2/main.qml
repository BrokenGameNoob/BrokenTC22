import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs

ApplicationWindow {
//    Material.theme: Material.Dark

    width: 640
    height: 480
    visible: true

    TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Home")
        }
        TabButton {
            text: qsTr("Discover")
        }
        TabButton {
            text: qsTr("Activity")
        }
    }

    StackLayout {
        currentIndex: bar.currentIndex

        anchors{
            top: bar.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Item {
            id: homeTab

            Button {
                id: button
                anchors.centerIn: parent
                text: qsTr("Hey")

                MessageDialog {
                    id: messagebox
                    buttons: MessageDialog.Cancel
                    text: "The document has been modified."
                }

                onClicked:{
                    messagebox.open();
                }
            }
            Switch {
                anchors.topMargin: 2
                anchors.top: button.bottom
                text: qsTr("Wi-Fi")
            }
        }
        Item {
            id: discoverTab
        }
        Item {
            id: activityTab
        }
    }

    title: qsTr("Hello World")
}
