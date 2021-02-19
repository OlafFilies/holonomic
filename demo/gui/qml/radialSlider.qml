import QtQuick 2.0

Item {

    id: origin
    property var range: [0,0]

    signal angleChanged(real a)

    Item {
        width: 0.5* origin.width
        height: origin.height
        anchors.right: parent.horizontalCenter
        anchors.rightMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        clip: true

        Item {
            width: origin.width
            height: origin.height
            anchors.left: parent.left
            anchors.leftMargin: 0
            rotation: origin.range[0]-180

            Item {
                width: 0.5* origin.width
                height: origin.height
                anchors.left: parent.left
                clip: true

                Rectangle {
                    width: origin.width
                    height: origin.height
                    radius: height*0.5
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    rotation: -parent.parent.rotation
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
                }
            }
        }
    }
    Item {
        width: 0.5* origin.width
        height: origin.height
        anchors.left: parent.horizontalCenter
        anchors.leftMargin: 0
        anchors.verticalCenter: parent.verticalCenter
        clip: true

        Item {
            width: origin.width
            height: origin.height
            anchors.right: parent.right
            anchors.rightMargin: 0
            rotation: origin.range[1]

            Item {
                width: 0.5* origin.width
                height: origin.height
                anchors.left: parent.left
                clip: true

                Rectangle {
                    width: origin.width
                    height: origin.height
                    radius: height*0.5
                    anchors.top: parent.top
                    anchors.topMargin: 0
                    anchors.left: parent.left
                    anchors.leftMargin: 0
                    rotation: -parent.parent.rotation
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
                }
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onPositionChanged: {
            if(pressed){
                var t = Math.atan2(mouseX-0.5*width, 0.5*height-mouseY) * 180/Math.PI;
                (t < origin.range[0]) ? origin.angleChanged(origin.range[0]) : (t > origin.range[1]) ? origin.angleChanged(origin.range[1]) : origin.angleChanged(t);
            }
        }
    }
}
