import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
 import QtQuick.Layouts 1.12
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
                slaveSel.model = ethercatmaster.getSlaveNameList();
                console.log("ethermaster.getSlaveNameList()",ethercatmaster.getSlaveNameList())
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
                    if(ethercatmaster.getSlaveCount() != slaveDeviceListModel.count){
                        return;
                    }
                    for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
                        var info = ethercatmaster.getSlaveInfo(i);
                        slaveDeviceListModel.setProperty(i,"ec_state",JSON.parse(info).ec_state)
//                        console.log("refreshSlaveState",i,info)
                    }
                }
            }
        }
        ComboBox{
            id:slaveSel
            width: 300
            onCurrentIndexChanged: {
                var slaveinfo = slaveDeviceListModel.get(slaveSel.currentIndex)
                slaveAllInfo.text = JSON.stringify(slaveinfo,null,2);
            }
        }
    }


    TabBar{
        id:slaveOptionHeader
        anchors.top: topHeader.bottom
        anchors.topMargin: 10
        anchors.left:topHeader.left
        width: parent.width - 20
        TabButton{
            text: qsTr("从站列表")
        }
        TabButton{
            text: qsTr("SDO读/写")
        }
        TabButton{
            text: qsTr("对象字典")
        }
        TabButton{
            text: qsTr("从站信息")
        }

    }

    StackLayout{
        width: slaveOptionHeader.width
        height:parent.height - topHeader.height - 30
        currentIndex:  slaveOptionHeader.currentIndex
        anchors.top: slaveOptionHeader.bottom
        anchors.topMargin: 10
        anchors.left:slaveOptionHeader.left
        Item {
            Rectangle{
                id:slaveDeviceListPage
                height: parent.parent.height
                width:  parent.parent.width

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
                            var info = ethercatmaster.getSlaveInfo(i);
                            var slaveInfo = JSON.parse(info);
                            slaveDeviceListModel.append(slaveInfo)
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
                                width: 450
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
                                width: 450
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

        }

        Item {
            Column{
                id:sdoOptionPage
                spacing: 10
//                x:10
//                y:10
//                anchors.top: topHeader.bottom
//                anchors.topMargin: 10
//                anchors.left:slaveDeviceListPage.right
//                anchors.leftMargin: 10
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
                        currentIndex: 2
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
                            if(slaveSel.currentIndex == -1){
                                dialog.waring(qsTr("SDO操作错误"),qsTr("请选择一个从站设备"))
                                return
                            }
                            var value = ethercatmaster.readSdo(slaveSel.currentIndex,mainIndex.intConfigValue(),subIndex.intConfigValue(),datatype.currentIndex)
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
                            if(slaveSel.currentIndex == -1){
                                dialog.waring(qsTr("SDO操作错误"),qsTr("请选择一个从站设备"))
                                return
                            }
                            ethercatmaster.writeSdo(slaveSel.currentIndex,mainIndex.intConfigValue(),subIndex.intConfigValue(),datatype.currentIndex,writeValue.intConfigValue())
                            if(ethercatmaster.getLastWorkCounter() <= 0){
                                dialog.waring(qsTr("SOD写操作失败"),qsTr("参数传入错误"))
                            }
                        }
                    }
               }
            }
        }
        Item {
            //< 对象字典
            ListView{
                id:slaveObjectDictionaryList
                model: ListModel{
                    id:slaveObjectDictionaryListModel
                }
                width: parent.parent.width
                height: parent.parent.height
                clip: true
                header: Rectangle{
                    width: parent.width
                    height: 30
                    border.color: "black"
                    border.width: 1
                    Row{
                        width: parent.width
                        height: parent.height
                        Text {
                            id: main_index_h
                            text: qsTr("索引")
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: sub_index_h
                            text: qsTr("子索引")
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: value_h
                            text: qsTr("值")
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: unit_h
                            text: qsTr("单位")
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: describe_h
                            text: qsTr("描述")
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
                delegate: Rectangle{
                    width: parent.width
                    height: 30
                    border.color: "black"
                    border.width: 1
                    Row{
                        width: parent.width
                        height: parent.height
                        Text {
                            id: main_index
                            text: main_index
                            width: main_index_h.width
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: sub_index
                            text: sub_index
                            width: sub_index_h.width
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: value
                            text: value
                            width: value_h.width
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: unit
                            text: unit
                            width: unit_h.width
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id: describe
                            text: describe
                            width: describe_h.width
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                }
            }
        }
        Item {
            //< 从站信息
            Column{
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
                            if(slaveDeviceListModel.count == 0){
                                dialog.waring(qsTr("SDO操作错误"),qsTr("请先初始化网卡"))
                                return
                            }
                            if(slaveSel.currentIndex == -1){
                                dialog.waring(qsTr("SDO操作错误"),qsTr("请选择一个从站设备"))
                                return
                            }
                            ethercatmaster.writeAlias(slaveSel.currentIndex,writeAliasValue.intConfigValue())
                        }
                    }
                }
                Text {
                    id:slaveAllInfo
                    width: parent.parent.width
                }
            }

        }
        onCurrentIndexChanged: {

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
