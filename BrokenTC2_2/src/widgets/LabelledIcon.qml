import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQml 2.15

import btc2

Item {
    id: root

    property bool iconOnTheRight: false
    property real spacerWidth: Style.kStandardMargin / 2

    property string color: Style.kForeground

    property alias source: icon.source
    property alias iconColor: icon.color
    property real iconSize: Style.kStandardIconWidth
    property alias iconWidth: icon.width
    property alias iconHeight: icon.height
    property alias text: lbl.text
    property alias font: lbl.font

    implicitWidth: rowLayout.implicitWidth
    implicitHeight: rowLayout.implicitHeight

    property bool fillParent: false

    property alias label: lbl

    RowLayout {
        id: rowLayout
        spacing: 0
        anchors.centerIn: parent
        anchors {
            centerIn: root.fillParent ? undefined : parent
            fill: root.fillParent ? parent : undefined
        }

        layoutDirection: iconOnTheRight ? Qt.RightToLeft : Qt.LeftToRight

        ColoredImage {
            id: icon
            color: root.color
            sourceSize.width: iconSize
            sourceSize.height: iconSize
            Layout.preferredWidth: width
            Layout.preferredHeight: height
            Layout.alignment: Qt.AlignVCenter
        }

        Item {
            id: spacer
            Layout.preferredWidth: spacerWidth
        }
        Label {
            id: lbl
            verticalAlignment: Text.AlignVCenter
        }
    }
}
