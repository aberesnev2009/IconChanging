import QtQuick
import QtQuick.Controls

import Test 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    property bool iconState: false

    Button {
        id: stateChangeBtn
        width: 320
        height: 240
        text: "Change taskbar icon"
        anchors.centerIn: parent
        onClicked: {
            if (iconState)
                iconState = false
            else
                iconState = true
            App.setMainWindowIconState(iconState)
        }
    }
}
