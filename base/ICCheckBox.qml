import QtQuick 2.0
import QtQuick.Controls 2.0

Item {
    id:root
    width: box.width+name.width
    height: 30

    property alias configName: name.text
    property alias configNameWidth:name.width
    property alias isCheck: box.checked
    property alias checkState: box.checkState

    signal click();

    CheckBox{
        id:box
        width: 30
        height: 30
        anchors.verticalCenter: parent.verticalCenter
        onClicked: {
            click()
        }
    }
    Text {
        id: name
//        text: qsTr("text")
        width: 50
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: box.right
        anchors.leftMargin: 3
    }
}
