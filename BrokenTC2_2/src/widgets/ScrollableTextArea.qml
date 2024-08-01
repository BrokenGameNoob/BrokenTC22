import QtQuick 2.15
import QtQuick.Layouts
import QtQuick.Controls

import btc2

ScrollView {
    id: scrollTextArea

    readonly property alias text: logArea.text

    /* Custom properties */
    readonly property real vBarPos: ScrollBar.vertical.position
    readonly property real vBarSize: ScrollBar.vertical.size
    readonly property bool vBarIsAtBottom: vBarSize + vBarPos >= 0.95

    /* Functions */
    function setText(txt: string) {
        let getBackDown = vBarIsAtBottom

        let vBarPosOld = vBarPos
        let vBarSizeOld = vBarSize

        debug1.text = "vPos: " + vBarPos + "   vSize: " + vBarSize

        logArea.text = txt
        if (getBackDown) {
            goToBottom()
            return
        }

        let vBarPosDiff = vBarPos - vBarPosOld
        let vBarSizeDiff = vBarSize - vBarSizeOld

        debug2.text = "vPos: " + vBarPos + "   vSize: " + vBarSize
        debug3.text = "vPosDiff: " + vBarPosDiff + "   vSizeDiff: " + vBarSizeDiff
        ScrollBar.vertical.position = vBarPosOld - vBarSizeDiff
    }
    function append(txt: string) {
        let getBackDown = vBarIsAtBottom

        let vBarPosOld = vBarPos
        let vBarSizeOld = vBarSize

        debug1.text = "vPos: " + vBarPos + "   vSize: " + vBarSize

        logArea.append(txt)
        if (getBackDown) {
            goToBottom()
            return
        }

        let vBarPosDiff = vBarPos - vBarPosOld
        let vBarSizeDiff = vBarSize - vBarSizeOld

        debug2.text = "vPos: " + vBarPos + "   vSize: " + vBarSize
        debug3.text = "vPosDiff: " + vBarPosDiff + "   vSizeDiff: " + vBarSizeDiff
        ScrollBar.vertical.position = vBarPosOld - vBarSizeDiff
    }

    function goToBottom() {
        ScrollBar.vertical.position = 1 - vBarSize
    }

    /* Layout */
    Layout.fillWidth: true
    Layout.fillHeight: true
    // Layout.columnSpan: grid.columns
    /* Scrollbar config */
    ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
    ScrollBar.vertical.policy: ScrollBar.AlwaysOn

    TextArea {
        id: logArea

        topInset: 0
        bottomInset: 0
        leftInset: 0
        rightInset: 0

        readOnly: true
        wrapMode: TextEdit.WordWrap
        textFormat: Text.RichText
        font.pointSize: 10
    }
}
