import QtQuick 2.0
import QtQuick.Controls 2.0
//Item {
//    property string ioName:""
//    property bool ioState: false
//    property int ioType: 0      //< 0：输入 1：输出

//    width: 40
//    height: stateRec.height+20

//    signal click(bool state)

//    Rectangle{
//        id:stateRec
//        width: 25
//        height: 25
//        border.color: "gray"
//        border.width: 1
//        color: ioState?(ioType==0?"red":"lime"):"gray"
//        anchors.verticalCenter: parent.verticalCenter
//        x:(parent.width - width)/2
//        MouseArea{
//            anchors.fill: parent
//            onClicked: {
//                if(ioType != 0){
//                    click(!ioState)
//                }
//            }
//        }
//    }
//    Text {
//        id: name
//        anchors.horizontalCenter:stateRec.horizontalCenter
//        anchors.top: stateRec.bottom
//        anchors.topMargin: 10
//        text: ioName
//    }
//


ICButton {
    property string ioName:""
    property bool ioState: false
    property int ioType: 0      //< 0：输入 1：输出

    bgColor: ioState?(ioType==0?"red":"lime"):"#c4c4c4"
    bdColor: "#777777"
    width: 40
    height: 40

    signal click(bool state)

    MouseArea{
        anchors.fill: parent
        onClicked: {
//            console.log('fds')
            if(ioType != 0){
                click(!ioState)
            }
        }
    }
    Text {
        id: name
        y: (parent.height - height) >> 1
        x: (parent.width - width) >> 1
        text: ioName
    }
}
