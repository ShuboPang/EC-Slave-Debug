import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
import"./base"
import "EthercatInfo.js" as EthercatInfoJs

ApplicationWindow {
    width: 1280
    height:768
    visible: true
    title: qsTr("EC-Slave-Debug")
    menuBar: MenuBar{
            Menu {
                 title: qsTr("&文件")
                 Action {
                     text: qsTr("&新建...")
                     shortcut : "Ctrl+N"
                     onTriggered: {
                        console.log("File New")
                     }

                 }
                 Action { text: qsTr("&打开...") }
                 Action { text: qsTr("&保存") }
                 Action { text: qsTr("另存为...") }
                 MenuSeparator { }
                 Action {
                     text: qsTr("&退出")
                     shortcut: "QKeySequence::Quit"
                     onTriggered: {

                     }
                 }
             }
            Menu {
                 title: qsTr("&脚本")
                 Action {
                     text: qsTr("&开始录制脚本...")

                 }
                 Action {
                     text: qsTr("&结束录制脚本...")
                 }
                 MenuSeparator { }
                 Action {
                     text: qsTr("&执行脚本...")
                 }
            }
            Menu {
                 title: qsTr("&设置")
                 Action {
                     text: qsTr("&配置网卡...")

                 }
                 Action {
                     text: qsTr("&语言...")
                 }
            }
            Menu {
                 title: qsTr("&帮助")
                 Action {
                     text: qsTr("&使用说明...")

                 }
                 Action {
                     text: qsTr("&伺服说明书...")
                 }
            }
            Menu {
                 title: qsTr("&关于")
                 Action {
                     text: qsTr("&关于...")
                     onTriggered: {
                     }

                 }
             }
        }

    Row{
        id:topHeader
        spacing: 10
        y:10
        x:10

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
                slaveDeviceList.refreshSlaveState()
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
            visible: false
            Timer{
                interval: 100
                running: true
                repeat: true;
                onTriggered: {
                    for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
//                    for(let i = 0;i<8;i++){
                        var info = ethercatmaster.getSlaveInfo(i);
                        slaveDeviceListModel.setProperty(i,"ec_state",JSON.parse(info).ec_state)
//                        console.log("refreshSlaveState",i,info)
                    }
                }
            }
        }
    }

    Rectangle{
        id:slaveDeviceListPage
        height: 600
        width: 570
        anchors.top: topHeader.bottom
        anchors.topMargin: 10
        anchors.left: topHeader.left

        border.color: "gray"
        border.width: 1

        ListView{
            id:slaveDeviceList
            model: ListModel{
                id:slaveDeviceListModel
            }
            width: parent.width
            height: parent.height

            clip: true

            function refreshSlaveState(){
                 slaveDeviceListModel.clear()
                for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
//                for(let i = 0;i<8;i++){
                    var info = ethercatmaster.getSlaveInfo(i);
                    var slaveInfo = JSON.parse(info);
                    slaveDeviceListModel.append(slaveInfo)
                    console.log("refreshSlaveState",i,info)
                }
            }

            header:Rectangle{
                width: parent.width
                height: 30
                border.color: "black"
                border.width: 1
                Row{
                    spacing: 3
                    height: parent.height
                    Text {
                        text: qsTr("从站名字")
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 220
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("厂家ID")
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 90
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("产品ID")
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 90
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("从站状态")
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 150
                    }
                }
            }
            delegate: Rectangle{
                width: parent.width
                height: 30
                border.color: "black"
                border.width: 1
                color: slaveDeviceList.currentIndex == index?"gray":"white"
                Row{
                    spacing: 3
                    height: parent.height
                    Text {
                        text: index+"   "+aliasadr+":"+SIIindex+" "+name
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 220
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: "0x"+eep_man.toString(16)
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        width: 90
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: "0x"+eep_id.toString(16)
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignRight
                        width: 90
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: "0x"+ec_state.toString(16)+"("+EthercatInfoJs.__EthercatSlaveStateEnum[ec_state]+")"
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                        width: 150
                    }
                }
                MouseArea{
                    anchors.fill:parent

                    acceptedButtons: Qt.LeftButton|Qt.RightButton

                    onClicked: {
                        slaveDeviceList.currentIndex = index;
                        if(mouse.button == Qt.RightButton){
                            option_menu.popup()
                        }
                    }

                    Menu{
                        id:option_menu
                        MenuItem{
                            text: qsTr("详细信息")

                        }
                        MenuItem{
                            text: qsTr("SDO")
                            onTriggered: {
                                sdoPage.visible = true
                            }
                        }
                    }
                }
            }
        }
    }
    Column{
        id:sdoOptionPage
        spacing: 10
        x:10
        y:10
        anchors.top: topHeader.bottom
        anchors.topMargin: 10
        anchors.left:slaveDeviceListPage.right
        anchors.leftMargin: 10
        Row{
            spacing: 10
            ICLineEdit{
                id:mainIndex
                configName:qsTr("主索引")
                isHex: true
                height: datatype.height
            }
            ICLineEdit{
                id:subIndex
                configName:qsTr("子索引")
                isHex: true
                height: datatype.height
            }
            ComboBox{
                id:datatype
                model: [qsTr("int8"),qsTr("int16"),qsTr("int32")]
                currentIndex: 1
                onVisibleChanged: {
                    if(visible){
                        model = EthercatInfoJs.__EthercatSlaveNameList;
                    }
                }
            }
        }
        Row{
            spacing: 10
            anchors.right: parent.right
            ICLineEdit{
                id:readvalue
                configName:qsTr("值")
//                isHex: true
                height: datatype.height
                enabled: false
                inputWidth: 150
            }
            Button{
                text: qsTr("读")
                onClicked: {
                    if(slaveDeviceListModel.count == 0){
                        dialog.waring(qsTr("SDO操作错误"),qsTr("请先初始化网卡"))
                        return
                    }
                    if(slaveDeviceList.currentIndex == -1){
                        dialog.waring(qsTr("SDO操作错误"),qsTr("请选择一个从站设备"))
                        return
                    }
                    var value = ethercatmaster.readSdo(slaveDeviceList.currentIndex,mainIndex.intConfigValue(),subIndex.intConfigValue(),datatype.currentIndex)
                    readvalue.configValue = value+"("+"0x"+value.toString(16)+")"
                    if(ethercatmaster.getLastWorkCounter() <= 0){
                        dialog.waring(qsTr("SOD读操作失败"),qsTr("参数传入错误"))
                    }
                }
            }
            ICLineEdit{
                id:writeValue
                configName:qsTr("值")
                height: datatype.height
            }
            Button{
                text: qsTr("写")
                onClicked: {
                    if(slaveDeviceListModel.count == 0){
                        dialog.waring(qsTr("SDO操作错误"),qsTr("请先初始化网卡"))
                        return
                    }
                    if(slaveDeviceList.currentIndex == -1){
                        dialog.waring(qsTr("SDO操作错误"),qsTr("请选择一个从站设备"))
                        return
                    }
                    ethercatmaster.writeSdo(slaveDeviceList.currentIndex,mainIndex.intConfigValue(),subIndex.intConfigValue(),datatype.currentIndex,writeValue.intConfigValue())
                    if(ethercatmaster.getLastWorkCounter() <= 0){
                        dialog.waring(qsTr("SOD写操作失败"),qsTr("参数传入错误"))
                    }
                }
            }
       }
//       Row{
//           spacing: 10
//           anchors.right: parent.right

//        }
    }

    Column{
        anchors.top: sdoOptionPage.bottom
        anchors.topMargin: 10
        anchors.left:slaveDeviceListPage.right
        anchors.leftMargin: 10
        Row{
            spacing: 10
            ICLineEdit{
                id:writeAliasValue
                configName: qsTr("逻辑地址")
                height: saveAliasBtn.height
            }
            Button{
                id:saveAliasBtn
                text: qsTr("保存")
                onClicked:  {
                    ethercatmaster.writeAlias(slaveDeviceList.currentIndex,writeAliasValue.intConfigValue())
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
