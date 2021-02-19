import QtQuick 2.2
//import QtBluetooth 5.3

Item {
    property string wldata: "";
    Rectangle {
        id: title
        height: titleLabel.height + 90
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 100

        color: "#5c9fba"
        Text {
            id: titleLabel
            text: "Select device"
            color: "black"
            font.pointSize: 15
            anchors.centerIn: parent
        }
    }

    ListView {
        id: listView
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: title.bottom
        anchors.topMargin: 0
        anchors.margins: 100
        clip: true
        add: Transition {
            NumberAnimation { properties: "x"; from: 1000; duration: 500 }
        }
        model: BluetoothDiscoveryModel {
            discoveryMode: BluetoothDiscoveryModel.DeviceDiscovery
            onErrorChanged: {
                if (error == BluetoothDiscoveryModel.NoError)
                    return;
                if (error == BluetoothDiscoveryModel.PoweredOffError)
                    titleLabel.text = "Bluetooth turned off";
                else
                    titleLabel.text = "Cannot find devices";
            }
            onServiceDiscovered: socket.service = service;
        }

        delegate: Button {
            width: listView.width + 2
            text: model.name
            onClicked:{
                //socket.service.deviceAddress = model.remoteAddress;
                root.remoteDevice = model.remoteAddress
            }
        }
    }
/*
    BluetoothSocket {
        id: socket
        connected: true

        onSocketStateChanged: {
            switch (socketState) {
                case BluetoothSocket.Unconnected:
                case BluetoothSocket.NoServiceSet:
                    serialPortInfo.error = "No connection to service";
                    break;
                case BluetoothSocket.Connected:
                    serialPortInfo.error = "connected";
                    break;
                case BluetoothSocket.Connecting:
                case BluetoothSocket.ServiceLookup:
                case BluetoothSocket.Closing:
                case BluetoothSocket.Listening:
                case BluetoothSocket.Bound:
                    break;
            }
        }
        onStringDataChanged: {
            console.log("Received data: " )
            var data = remoteDeviceName + ": " + socket.stringData;
            data = data.substring(0, data.indexOf('\n'))
            chatContent.append({content: data})
            console.log(data);
        }
    }
*/
    function write(p){
        socket.stringData = p;
    }
}
