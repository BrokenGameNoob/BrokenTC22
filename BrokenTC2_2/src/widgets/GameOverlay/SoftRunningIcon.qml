import QtQuick 2.15

import ".."
import "../../utils"
import "../../widgets"

import btc2

Item {
    width: 20
    height: 20

    Rectangle {
        color: QMLStyle.kBackgroundColor
        anchors.fill: btc2LaunchedIcon
        anchors.margins: -QMLStyle.kStandardMargin
        opacity: 0.7
        radius: width / 2
        visible: btc2LaunchedIcon.visible
    }

    ColoredImage {
        id: btc2LaunchedIcon
        source: Constants.kIconUbiNope
        anchors.fill: parent
        sourceSize.width: width
        sourceSize.height: height
        color: QMLStyle.kAccentColor
        visible: !content.visible
                 && ServiceManager.settings.LaunchedOverlayEnabled
    }
}
