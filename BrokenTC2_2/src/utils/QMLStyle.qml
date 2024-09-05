pragma Singleton

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Item {
    readonly property real kRadius: Material.roundedScale
    //    readonly property real kBorderWeight: Material.1
    readonly property color kBorderColor: Material.frameColor

    readonly property real kStandardMargin: 10

    readonly property font kFontH2: ({
                                         "pointSize": 15,
                                         "bold": true
                                     })

    readonly property font kFontH3: ({
                                         "pointSize": 10,
                                         "bold": true
                                     })
}
