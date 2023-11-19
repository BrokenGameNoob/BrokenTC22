import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15

import btc2

Item {
    id: root

    property string color: Style.kForeground

    property alias source: icon.source
    property alias iconColor: icon.color
    property real iconSize: Style.kStandardIconWidth
    property alias iconWidth: icon.width
    property alias iconHeight: icon.height
    property alias text: lbl.text

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    RowLayout {
        id: rowLayout
        spacing: 5
        anchors.centerIn: parent

        IconImage {
            id: icon
            color: root.color
            width: iconSize * 1.8
            height: iconSize * 1.8
            Layout.preferredWidth: width
            Layout.preferredHeight: height
            onWidthChanged: {
                console.warn("Width=", width)
            }
        }

        Label {
            id: lbl
            //            color: root.color
            verticalAlignment: Text.AlignVCenter
        }
    }
}
