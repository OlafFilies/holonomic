import QtQuick 2.1

Rectangle {
    property alias text: label.text
    signal clicked();
    opacity: 0.7
    height: label.height + 30
    width: label.width + 90
    border.color: Qt.lighter("#67b0d1")
    border.width: 1
    color: mArea.pressed ? "#5c9fba" : "#67b0d1"
    Text {
        id: label
        anchors.centerIn: parent
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        font.pointSize: 10
        color: "black"
    }
    MouseArea {
        id: mArea
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
