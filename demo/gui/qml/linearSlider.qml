import QtQuick 2.0

Item {

    id: origin
    property var range: [0,0]
    property real value: 0
    signal posChanged(real a)

    Rectangle {
        anchors.fill: parent
        radius: height*0.1
        gradient: Gradient {
            GradientStop {
                position: 0
                color: "#1489e0"
            }

            GradientStop {
                position: 1
                color: "#0444a9"
            }
        }

        Rectangle {
            color: "#fff"
            width: 1
            height: 0.7* parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.1* parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            color: "#fff"
            width: 1
            height: 0.4* parent.height
            anchors.left: parent.left
            anchors.leftMargin: 0.3* parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            color: "#fff"
            width: 1
            height: 0.8* parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            color: "#fff"
            width: 1
            height: 0.4* parent.height
            anchors.right: parent.right
            anchors.rightMargin: 0.3* parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
        Rectangle {
            color: "#fff"
            width: 1
            height: 0.7* parent.height
            anchors.right: parent.right
            anchors.rightMargin: 0.1* parent.width
            anchors.verticalCenter: parent.verticalCenter
        }

        Rectangle {
            color: "#f5c345"
            width: 2
            height: 0.8* parent.height
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.horizontalCenterOffset: 0.004* origin.value*parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
    }
    MouseArea {
        anchors.fill: parent
        cursorShape: Qt.PointingHandCursor

        onPositionChanged: {
            if(pressed){
                var m = Math.round(1.25* (origin.range[1]-origin.range[0])* (mouseX/width-0.1)) + origin.range[0];
                origin.value = (m > origin.range[1]) ? origin.range[1] : (m < origin.range[0]) ? origin.range[0] : m;
                origin.posChanged(origin.value);
            }
        }
        onReleased: {
            origin.value = 0;
            origin.posChanged(0);
        }
    }
}
