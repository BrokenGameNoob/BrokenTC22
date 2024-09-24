import QtQuick 2.15
import Qt5Compat.GraphicalEffects

import ".."
import "../utils"
import "../widgets"

import btc2

Rectangle {
    id: loadingCenter

    required property color backgroundColor
    required property color foregroundColor
    property real animationDuration: 1200
    property real animationDurationRatio: 0.7

    readonly property real widthRatio: 0.5
    readonly property real midAngle: 360 / 2

    color: loadingCenter.backgroundColor
    radius: width / 2
    clip: true

    Rectangle {
        anchors.centerIn: loadingCenter
        width: loadingCenter.width * loadingCenter.widthRatio
        height: loadingCenter.height * 1.5
        color: loadingCenter.foregroundColor
        clip: true
        SequentialAnimation on rotation {
            loops: Animation.Infinite
            RotationAnimation {
                from: 0
                to: loadingCenter.midAngle
                duration: loadingCenter.animationDuration
                          * (1 - loadingCenter.animationDurationRatio)
            }
            RotationAnimation {
                from: loadingCenter.midAngle
                to: 360
                duration: loadingCenter.animationDuration * loadingCenter.animationDurationRatio
            }
        }
    }

    layer.enabled: true
    layer.effect: OpacityMask {
        maskSource: Rectangle {
            width: loadingCenter.width
            height: loadingCenter.height
            radius: loadingCenter.radius
        }
    }
}
