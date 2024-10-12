// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR LGPL-3.0-only OR GPL-2.0-only OR GPL-3.0-only
// Modified by BrokenGameNoob 2024
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs
import QtQuick.Dialogs.quickimpl

import ".."
import "../utils"
import "../widgets"

import btc2

ThemedPopup {
    id: control
    modal: true
    anchors.centerIn: Overlay.overlay
    objectName: "colorDialog"
    width: Math.min(Overlay.overlay.width / 2., 600)
    height: Math.min(Overlay.overlay.height / 1.5, 500)

    property bool handled: false
    property color color
    property bool colorBeingChanged: false
    property real hue: 0
    property real alpha: 1

    property var callback: null

    onColorChanged: {
        control.colorBeingChanged = true
        if (!colorPicker.updatingColor) {
            colorPicker.color = control.color
        }
        if (!inputs.updatingColor) {
            inputs.color = control.color
        }

        control.hue = control.color.hslHue
        control.alpha = control.color.a
        control.colorBeingChanged = false
    }

    onHueChanged: {
        if (control.colorBeingChanged) {
            return
        }
        control.color = Qt.hsla(hue, colorPicker.saturation,
                                colorPicker.lightness, alpha)
    }

    onAlphaChanged: {
        if (control.colorBeingChanged) {
            return
        }
        control.color = Qt.hsla(hue, colorPicker.saturation,
                                colorPicker.lightness, alpha)
    }

    signal selectedColor(var color)
    signal rejected

    function selectColor(initialColor, _callback) {
        control.callback = _callback
        control.color = initialColor
        control.open()
    }

    Item {
        anchors.fill: parent

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: QMLStyle.kStandardMargin * 2
            spacing: 12
            SaturationLightnessPicker {
                id: colorPicker
                objectName: "colorPicker"

                Layout.fillWidth: true
                Layout.fillHeight: true

                property bool updatingColor: false

                onColorChanged: {
                    colorPicker.updatingColor = true
                    control.color = colorPicker.color
                    colorPicker.updatingColor = false
                }
            }

            Slider {
                id: hueSlider
                objectName: "hueSlider"
                orientation: Qt.Horizontal
                value: control.hue
                implicitHeight: 20
                onMoved: function () {
                    control.hue = value
                }
                handle: PickerHandle {
                    x: hueSlider.leftPadding + (hueSlider.horizontal ? hueSlider.visualPosition * (hueSlider.availableWidth - width) : (hueSlider.availableWidth - width) / 2)
                    y: hueSlider.topPadding
                       + (hueSlider.horizontal ? (hueSlider.availableHeight - height)
                                                 / 2 : hueSlider.visualPosition
                                                 * (hueSlider.availableHeight - height))
                    picker: hueSlider
                }
                background: Rectangle {
                    anchors.fill: parent
                    anchors.leftMargin: hueSlider.handle.width / 2
                    anchors.rightMargin: hueSlider.handle.width / 2
                    border.width: 2
                    border.color: control.palette.dark
                    radius: 10
                    color: "transparent"
                    Rectangle {
                        anchors.fill: parent
                        anchors.margins: 4
                        radius: 10
                        gradient: HueGradient {
                            orientation: Gradient.Horizontal
                        }
                    }
                }

                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
            }

            Slider {
                id: alphaSlider
                objectName: "alphaSlider"
                orientation: Qt.Horizontal
                value: control.alpha
                onMoved: function () {
                    control.alpha = value
                }
                implicitHeight: 20
                handle: PickerHandle {
                    x: alphaSlider.leftPadding + (alphaSlider.horizontal ? alphaSlider.visualPosition * (alphaSlider.availableWidth - width) : (alphaSlider.availableWidth - width) / 2)
                    y: alphaSlider.topPadding
                       + (alphaSlider.horizontal ? (alphaSlider.availableHeight - height)
                                                   / 2 : alphaSlider.visualPosition
                                                   * (alphaSlider.availableHeight - height))
                    picker: alphaSlider
                }
                background: Rectangle {
                    anchors.fill: parent
                    anchors.leftMargin: parent.handle.width / 2
                    anchors.rightMargin: parent.handle.width / 2
                    border.width: 2
                    border.color: control.palette.dark
                    radius: 10
                    color: "transparent"

                    Image {
                        anchors.fill: alphaSliderGradient
                        source: "qrc:/qt-project.org/imports/QtQuick/Dialogs/quickimpl/images/checkers.png"
                        fillMode: Image.Tile
                    }

                    Rectangle {
                        id: alphaSliderGradient
                        anchors.fill: parent
                        anchors.margins: 4
                        radius: 10
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop {
                                position: 0
                                color: "transparent"
                            }
                            GradientStop {
                                position: 1
                                color: Qt.rgba(control.color.r,
                                               control.color.g,
                                               control.color.b, 1)
                            }
                        }
                    }
                }

                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
            }

            ColorInputs {
                id: inputs

                showAlpha: true

                property bool updatingColor: false

                onColorModified: color => {
                                     inputs.updatingColor = true
                                     control.color = color
                                     inputs.updatingColor = false
                                 }

                Layout.fillWidth: true
                Layout.leftMargin: 12
                Layout.rightMargin: 12
                Layout.bottomMargin: 12
            }

            RowLayout {
                Item {
                    Layout.preferredWidth: QMLStyle.kStandardTitleIconSize
                    Layout.preferredHeight: QMLStyle.kStandardTitleIconSize
                    Image {
                        anchors.fill: parent
                        source: "qrc:/qt-project.org/imports/QtQuick/Dialogs/quickimpl/images/checkers.png"
                        fillMode: Image.Tile
                    }
                    Rectangle {
                        anchors.fill: parent
                        color: control.color
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                ButtonOk {
                    text: qsTr("Ok")
                    onClicked: {
                        control.selectedColor(control.color)
                        if (control.callback) {
                            control.callback(control.color)
                        }
                        control.close()
                    }
                }

                Item {
                    Layout.preferredWidth: QMLStyle.kStandardMargin
                }

                ButtonCancel {
                    text: qsTr("Cancel")
                    onClicked: {
                        control.rejected()
                        control.close()
                    }
                }
            }
        }
    }
}
