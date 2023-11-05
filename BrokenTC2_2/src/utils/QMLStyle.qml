pragma Singleton

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Item {
    readonly property real kRadius: Material.roundedScale
    //    readonly property real kBorderWeight: Material.1
    readonly property color kBorderColor: Material.frameColor


    /*
    //
    //
    */
    visible: false
    enabled: false
    Frame {
        id: dummyControl
        visible: false
        enabled: false
    }
}
