
import QtQuick 2.2

Item {
    id: root

    property Item clip
    property bool mirrorGradient: false

    width: 100
    height: clip ? clip.height : 0
    Behavior on opacity { NumberAnimation { duration: 100 } }

    Rectangle {
        id: shadowGradient
        width: parent.height
        height: parent.width
        anchors.centerIn: parent
        rotation: mirrorGradient ? -90 : 90
        gradient: Gradient {
            GradientStop { position: 0.0; color: "transparent" }
            GradientStop { position: 1.0; color: "white" }
        }
        PulsingAnimation {
            target: shadowGradient
            running: root.opacity
        }
    }
}
