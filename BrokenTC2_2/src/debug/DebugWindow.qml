import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import btc2
import "../widgets"

Rectangle {
    id: root
    color: "black"

    height: 100

    QMLLogHelper {
        id: qmlLogHelper
        onTextAdded: function (addedText) {
            scrollTextArea.append(addedText)
        }
    }

    ColumnLayout {
        anchors.fill: parent
        ScrollableTextArea {
            id: scrollTextArea
        }

        RowLayout {
            Text {
                id: debug1
                color: "white"
                text: "DEBUG 1"
            }
            Text {
                id: debug2
                color: "white"
                text: "DEBUG 2"
            }
            Text {
                id: debug3
                color: "white"
                text: "DEBUG 3"
            }
            Button {
                text: "TEST"
                onClicked: {
                    ServiceManager.test()
                }

                height: 20
                Layout.alignment: Qt.AlignRight
                Layout.rightMargin: 10
            }
            Text {
                id: debug4
                color: "white"
                text: "DEBUG name=" + ServiceManager.dummy.Name
            }
        }
    }
}
