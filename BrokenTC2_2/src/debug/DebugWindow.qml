import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import btc2
import "../widgets"

Rectangle {
    id: root
    color: "black"

    height: 100

    QMLLogHelper{
        id: qmlLogHelper
        onTextAdded: function(addedText){
            scrollTextArea.append(addedText);
        }
    }

    GridLayout {
        id: grid

        anchors.fill: parent

        columns: 3

        ScrollableTextArea{
            id: scrollTextArea
        }

        Text {
            id: debug1
            color: "white"
            text: "DEBUG " + scrollTextArea.vBarIsAtBottom
        }
        Text {
            id: debug2
            color: "white"
            text: "DEBUG " + scrollTextArea.vBarIsAtBottom
        }
        Text {
            id: debug3
            color: "white"
            text: "DEBUG " + scrollTextArea.vBarIsAtBottom
        }
    }
}
