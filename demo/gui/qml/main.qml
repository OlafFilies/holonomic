import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtLocation 5.6
import Serial 1.0

ApplicationWindow {
    property string bg: "#424241"
    // property string imgSrc: "/home/marcus/workspace/robot/icons/"
    id: window
    visible: true
    width: 640
    height: 480
    color: bg
    title: qsTr("XMC robot controller")
    menuBar: MenuBar {
       Menu {
           title: "Serial Ports"
           id: serialPortList
           onAboutToShow: serialPortInfo.serialLists();
       }
       Menu {
           title: "Bluetooth"
           onAboutToShow: root.visible = true;
       }
    }

    Serial {
        id: serialPortInfo;
        property string portName: "NULL";
        property string error: "";
        property var lock: ["<0,0,0>", true, ""];

        function serialLists(){
            var portList = getPInfo();
            for(var i = 0; i < serialPortList.items.length; i++){
                serialPortList.removeItem(serialPortList.items[i]);
            }
            for(i = 0; i < portList.length; i++){
                console.log(portList[i]);
                var el = serialPortList.addItem(portList[i]);
                el.cb = i;
                el.action = menuTrigger;
            }
        }

        function writePort(){
            if(lock[1]){
                writePortD();
            }
        }

        function writePortD(){
            var p = "<"+dirSlider.value+","+speedSlider.value+","+rotSlider.value+">";
            if(p != lock[0]){
                if(root.remoteDevice){
                    qb.item.write(p);
                } else {
                    lock[1] = false;
                    write(p);
                }
            }
        }

        onRead: {
            lock[2] += readContent;
            if(readContent[readContent.length-1] == ">"){
                lock[1] = true;
                error = lock[2];
                lock[2] = "";
            }
        }
        onSerialError: {
            error = errorMessage;
        }

    }

    Action {
        id: menuTrigger;
        onTriggered: {
            serialPortInfo.lock[1] = true;
            serialPortInfo.portName = serialPortInfo.selectPort(source.cb);
        }
    }

    Text {
        id: portText
        width: parent.width
        height: 104
        color: "#ffffff"
        text: serialPortInfo.portName;
        anchors.top: parent.top
        anchors.topMargin: 0
        font.pixelSize: 20
    }

    Text {
        id: errorText
        y: 376
        width: parent.width
        height: 104
        color: "#f78e3a"
        text: serialPortInfo.error;
        verticalAlignment: Text.AlignBottom
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        font.pixelSize: 20
    }

    Loader {
        id: rotSlider
        source: "linearSlider.qml"
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: controllArea.left
        anchors.rightMargin: 10
        height: 0.3* width
        anchors.verticalCenter: parent.verticalCenter
        property real value: 0

        onLoaded: {
            item.range = [-100,100];
        }
        Connections {
            target: rotSlider.item

            onPosChanged: {
                rotSlider.value = Math.floor(a);
                (rotSlider.value == 0) ? serialPortInfo.writePortD() : serialPortInfo.writePort();
            }
        }
    }

    Slider {
        id: dirSlider
        width: rotSlider.width
        anchors.left: rotSlider.left
        anchors.leftMargin: 0
        anchors.top: rotSlider.bottom
        anchors.topMargin: 2
        stepSize: 1
        maximumValue: 359
        orientation: Qt.Horizontal
        updateValueWhileDragging: true
        value: 0
    }

    Slider {
        id: speedSlider
        width: rotSlider.width
        anchors.left: rotSlider.left
        anchors.leftMargin: 0
        anchors.top: dirSlider.bottom
        anchors.topMargin: 2
        stepSize: 1
        maximumValue: 100
        orientation: Qt.Horizontal
        updateValueWhileDragging: true
        value: 0
    }

    Item {
        id: controllArea
        height: 0.8* parent.height
        width: height
        anchors.top: parent.top
        anchors.topMargin: 0.5* (parent.height-height)
        anchors.right: parent.right
        anchors.rightMargin: 10

        Loader {
            source: "radialSlider.qml"
            anchors.fill: parent

            onLoaded: {
                item.range = [-180,180];
            }
        }

        Text {
            text: "↑"
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: width
            font.bold: true
            width: 0.125* parent.width
            height: width
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: -0.5* width
            anchors.top: parent.top
            anchors.topMargin: height
        }
        Text {
            text: "→"
            verticalAlignment: Text.AlignBottom
            font.pixelSize: width
            font.bold: true
            width: 0.125* parent.width
            height: width
            anchors.right: parent.right
            anchors.rightMargin: width
            anchors.top: parent.verticalCenter
            anchors.topMargin: -0.5* height
        }
        Text {
            text: "↓"
            verticalAlignment: Text.AlignTop
            horizontalAlignment: Text.AlignHCenter
            font.pixelSize: width
            font.bold: true
            width: 0.125* parent.width
            height: width
            anchors.left: parent.horizontalCenter
            anchors.leftMargin: -0.5* width
            anchors.bottom: parent.bottom
            anchors.bottomMargin: height
        }
        Text {
            text: "←"
            verticalAlignment: Text.AlignBottom
            font.pixelSize: width
            font.bold: true
            width: 0.125* parent.width
            height: width
            anchors.left: parent.left
            anchors.leftMargin: width
            anchors.top: parent.verticalCenter
            anchors.topMargin: -0.5* height
        }

        MouseArea {
            id: clickArea
            anchors.fill: parent
            width: parent.width
            height: parent.height
            cursorShape: (mDist < rad) ? Qt.PointingHandCursor : Qt.ArrowCursor

            property double xPos: 0.5* width - mouseX
            property double yPos: 0.5* height - mouseY
            property double mDist: Math.pow(xPos, 2) + Math.pow(yPos, 2)
            property double rad: Math.pow(0.5* Math.min(width, height), 2)
            readonly property bool mouseDown: pressed && mDist < rad

            onPositionChanged: {
                if(pressed){
                    var l = Math.round( 200* Math.sqrt( mDist ) / width );
                    var a = Math.round( 180* Math.atan( yPos/xPos )/ Math.PI );
                    if(xPos >= 0)
                        a += 180;
                    else if(yPos > 0)
                        a += 360;
                    speedSlider.value = (l > 100) ? 100 : l;
                    dirSlider.value = 360-a;
                    serialPortInfo.writePort();
                }
            }
            onReleased: {
                speedSlider.value = 0;
                serialPortInfo.writePortD();
            }
        }
    }

    Rectangle {
        id: root
        visible: false
        anchors.fill: parent
        color: window.bg
        property string remoteDevice;

        onRemoteDeviceChanged: {
            visible = false;
            serialPortInfo.portName = remoteDevice;
        }

        Loader {
            id: qb
            source: "wl.qml"
            anchors.fill: parent
        }

        Button {
            text: "Close"
            anchors.top: parent.top
            anchors.topMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            onClicked: root.visible = false;
        }
    }

}
