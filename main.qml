import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5

Window {
    width: 1280
    height:768
    visible: true
    title: qsTr("EC-Slave-Debug")


    Row{
        id:topHeader
        spacing: 10
        ComboBox{
            id:networkList
            width: 300
        }
        Button{
            text: qsTr("初始化")
            onClicked: {
                if(networkList.currentIndex == -1){
                    return;
                }
                let ret = ethercatmaster.init(networkList.currentIndex)
                console.log("ethercat master init ret = ",ret);
                if(ret == 0){
                    dialog.waring(qsTr("初始化"),qsTr("识别到从站数量：")+ethercatmaster.getSlaveCount())
                    slaveDeviceList.refreshSlaveState()
                }
                else if(ret == -1){
                    dialog.waring(qsTr("初始化失败"),qsTr("网卡不存在"))
                }
                else if(ret == -2){
                    dialog.waring(qsTr("初始化失败"),qsTr("网卡初始化失败"))
                }
                else if(ret == -3){
                    dialog.waring(qsTr("初始化失败"),qsTr("未找到Ethercat从站设备"))
                }
            }
        }
        Button{
            text:qsTr("刷新")
            onClicked: {
                slaveDeviceList.refreshSlaveState()
            }
        }
        CheckBox{
            text: qsTr("自动刷新")
            Timer{
                interval: 100
                running: parent.checked
                repeat: true;
                onTriggered: {
                    slaveDeviceList.refreshSlaveState()
                }
            }
        }
    }
    ListModel{
        id:slaveDeviceListModel
    }
    Rectangle{
        height: 600
        width: 1000
        anchors.top: topHeader.bottom
        anchors.topMargin: 10

        border.color: "gray"
        border.width: 1

        ListView{
            id:slaveDeviceList
            model: slaveDeviceListModel
            width: parent.width
            height: parent.height

            function refreshSlaveState(){
                for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
                    var info = ethercatmaster.getSlaveInfo(i);
                    slaveDeviceListModel.append(JSON.parse(info))
                    console.log(info)
                }
            }

            delegate: Rectangle{
                width: parent.width
                height: 30
                border.color: "black"
                border.width: 1
                color: slaveDeviceList.currentIndex == index?"lime":"white"
                Row{
                    spacing: 3
                    height: parent.height
                    Text {
                        text: name
                        anchors.verticalCenter: parent.verticalCenter
                        width: 200
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: eep_id
                        anchors.verticalCenter: parent.verticalCenter
                        width: 150
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: eep_man
                        anchors.verticalCenter: parent.verticalCenter
                        width: 150
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: state
                        anchors.verticalCenter: parent.verticalCenter
                        width: 150
                    }
                }
                MouseArea{
                    anchors.fill:parent
                    onClicked: {
                        slaveDeviceList.currentIndex = index;
                    }
                }
            }
        }
    }
    Dialog{
        id:dialog
        anchors.centerIn: parent
        Text {
            id: message
        }

        onAccepted: console.log("Ok clicked")

        function waring(text1,text2){
            dialog.title = text1;
            message.text = text2
            dialog.standardButtons = Dialog.Ok
            dialog.visible= true

        }
    }





    Component.onCompleted: {
        networkList.model = ethercatmaster.scanNetwork();

    }
}
