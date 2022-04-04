import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5

Rectangle {
    id:root
    property alias configName: name.text
    property alias configNameWidth: name.width
    property alias unitText:unit.text
    property alias items: com.model
    property alias currentIndex: com.currentIndex

    property alias inputWidth: com.width
    height: root.height
    width:name.width+ com.width+unit.width+10

    signal editFinsih()

    function getCurrentText(){
        return items[currentIndex]
    }

    function getCurrentIndex(){
        return currentIndex
    }

    Row{
        //y:3
        //x:5
        height: parent.height
        //anchors.verticalCenter: parent.verticalCenter
        Text {
            id: name
            height: root.height
            //anchors.verticalCenter: com.verticalCenter
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }
        ComboBox{
            id:com
//            height: root.height
//            width: 100

        }
        Text {
            id: unit
            height: root.height
            anchors.verticalCenter: com.verticalCenter
        }

    }



}
