pragma Singleton

import QtQuick
import QtQuick.Controls.impl
import QtQuick.Controls.Material
import QtQuick.Controls.Material.impl

Item {
    readonly property real kRadius: 7. //Material.roundedScale
    //    readonly property real kBorderWeight: Material.1
    readonly property color kBorderColor: Material.frameColor
    readonly property color kBackgroundColor: Material.backgroundColor
    readonly property color kAccentColor: Material.accentColor
    readonly property color kIconColor: Material.iconColor
    readonly property color kPrimaryColor: Material.primaryColor
    readonly property color kTextColor: Material.primaryTextColor

    readonly property color kButtonColor: Material.buttonColor(
                                              Material.theme /* Theme */
                                              ,
                                              Material.backgroundColor /* Background color */
                                              ,
                                              Material.accentColor /* Accent color */
                                              , true /* enabled */
                                              , false /* flat */
                                              , false /* highlighted */
                                              , false /* checked */
                                              )

    readonly property color kErrorRed: "#ff1f1f"
    readonly property color kCancelColor: kErrorRed
    readonly property color kOkColor: "#35ff1f"

    function buttonColor(enabled, highlighted, checked, flat = false) {
        return Material.buttonColor(Material.theme, Material.backgroundColor,
                                    Material.accentColor, enabled, flat,
                                    highlighted, checked)
    }

    readonly property real kStandardMargin: 10
    readonly property real kStandardTitleIconSize: 35

    readonly property font kFontGearHandler: ({
                                                  "pointSize": 56,
                                                  "bold": true
                                              })

    readonly property font kFontH2: ({
                                         "pointSize": 17,
                                         "bold": false
                                     })

    readonly property font kFontH2Bold: ({
                                             "pointSize": 17,
                                             "bold": true
                                         })

    readonly property font kFontH3: ({
                                         "pointSize": 14,
                                         "bold": false
                                     })

    readonly property font kFontH3Bold: ({
                                             "pointSize": 14,
                                             "bold": true
                                         })

    readonly property font kFontH4: ({
                                         "pointSize": 10,
                                         "bold": false
                                     })

    readonly property font kFontH4Bold: ({
                                             "pointSize": 10,
                                             "bold": true
                                         })
}
