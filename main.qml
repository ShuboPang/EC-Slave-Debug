import QtQuick 2.12
import QtQuick.Window 2.12
import QtQml 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls 2.5
 import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.3
import"./base"
import "EthercatInfo.js" as EthercatInfoJs

ApplicationWindow {
    width: 1280
    height:768
    visible: true
    title: qsTr("EC-Slave-Debug")+"-"+"1.0.3"+"-"+"20221025001"
    menuBar: MenuBar{
            Menu {
                 title: qsTr("&伺服参数")
                 Action { text: qsTr("&打开伺服参数文件...")
                    shortcut: "Ctrl+O"
                    onTriggered: {
                        openFileDialog.visible = true;
                    }
                 }
                 Action { text: qsTr("&保存伺服参数")
                     shortcut: "Ctrl+s"
                    onTriggered: {
                        outputFileDialog.visible = true;
                    }
                 }
                 MenuSeparator { }
                 Action {
                     text: qsTr("&退出")
                     shortcut: "QKeySequence::Quit"
                     onTriggered: {
                        Qt.quit()
                     }
                 }
             }
            Menu {
                 title: qsTr("&SII")
                 Action { text: qsTr("&打开SII文件...")
                     shortcut: "Ctrl+i"
                    onTriggered: {
                        openFileDialog_alias.visible = true;
                    }
                 }
                 Action { text: qsTr("&保存SII")
                     shortcut: "Ctrl+j"
                    onTriggered: {
                        outputFileDialog_alias.visible = true;
                    }
                 }
            }
            Menu {
                 title: qsTr("&从站升级")
                 Action { text: qsTr("&打开升级文件...")
                     shortcut: "Ctrl+u"
                    onTriggered: {
                        openFileDialog_firmware.open()
                    }
                 }
                 Action { text: qsTr("&FOE升级...")
                     shortcut: "Ctrl+l"
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
        Text {
            text: qsTr("网卡选择:")
            height: networkList.height
            anchors.verticalCenter: networkList.verticalCenter
            verticalAlignment: Text.AlignVCenter
        }
        ComboBox{
            id:networkList
            width: 450
        }
        Button{
            text: qsTr("初始化")
            enabled:networkList.enabled
            onClicked: {
                slaveSel.currentIndex = -1
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
                esdSlaveSel.model = ethercatmaster.getSlaveNameList();
                siiSlaveSel.model = ethercatmaster.getSlaveNameList();
                firmwareSlaveSel.model = siiSlaveSel.model
                console.log("ethermaster.getSlaveNameList()",ethercatmaster.getSlaveNameList())
                if(ethercatmaster.getSlaveCount()){
                    slaveSel.currentIndex = 0
                }
//                networkList.enabled = false;
            }
        }
        Text {
            text: qsTr("从站选择:")
            height: slaveSel.height
            anchors.verticalCenter: slaveSel.verticalCenter
            verticalAlignment: Text.AlignVCenter
        }
        ComboBox{
            id:slaveSel
            width: 300
            onCurrentIndexChanged: {
                if(slaveSel.currentIndex == -1){
                    return;
                }
                var slaveinfo = slaveDeviceListModel.get(slaveSel.currentIndex)
                slaveAllInfo.text = JSON.stringify(slaveinfo,null,2);
                slaveSiiInfo.text = ethercatmaster.readSII(slaveSel.currentIndex)
                // 载入对象字典
                slaveObjectDictionaryListModel.clear()
                var slave_ = EthercatInfoJs.ethercatSlaveDeviceFind(slaveinfo.eep_man,slaveinfo.eep_id);
                if(slave_ != undefined){

                    let slave_sdo = slave_.sdo
                    for(let i = 0;i<slave_sdo.length;i++){
                        let size = 0;
                        if(slave_sdo[i].type == 2){
                            size = 1;
                        }
                        else if(slave_sdo[i].type == 4){
                            size = 2;
                        }
                        let  value = ethercatmaster.readSdo(slaveSel.currentIndex,slave_sdo[i].main_index,slave_sdo[i].sub_index,size)
                        slaveObjectDictionaryListModel.append({"main_index":slave_sdo[i].main_index,"sub_index":slave_sdo[i].sub_index,"value":value,
                                                              "unit":slave_sdo[i].unit,"describe":slave_sdo[i].describe,"type":slave_sdo[i].type
                                                              });
                    }
                    if(slave_.axis_num == 2){
                        axis_2_control_page.visible = true
                    }
                    else{
                        axis_2_control_page.visible = false
                    }
                    axis_1_control_velocity_unit.text = slave_.velocity_unit
                }

            }
        }
        Button{
            text:qsTr("刷新")
            onClicked: {
                if(slaveOptionHeader.currentIndex == 2){
                    //< 对象字典刷新
                    if(slaveObjectDictionaryListModel.count == 0){
                        return;
                    }
                    for(let i = 0;i<slaveObjectDictionaryListModel.count;i++){
                        let  slave_sdo = slaveObjectDictionaryListModel.get(i);
                        let size = 0;
                        if(slave_sdo.type == 2){
                            size = 1;
                        }
                        else if(slave_sdo.type == 4){
                            size = 2;
                        }
                        let  value = ethercatmaster.readSdo(slaveSel.currentIndex,slave_sdo.main_index,slave_sdo.sub_index,size)
                        slaveObjectDictionaryListModel.setProperty(i,"value",value);
                    }

                }
                else if(slaveOptionHeader.currentIndex == 5){
                    var slaveinfo = slaveDeviceListModel.get(slaveSel.currentIndex)
                    slaveAllInfo.text = JSON.stringify(slaveinfo,null,2);
                    slaveSiiInfo.text = ethercatmaster.readSII(slaveSel.currentIndex)
                }
            }
        }
        Button{
            text:qsTr("写EEPROM")
            onClicked: {
                if(slaveSel.currentIndex == -1){
                    return;
                }
                let slave_info = slaveDeviceListModel.get(slaveSel.currentIndex);
                var slave = EthercatInfoJs.ethercatSlaveDeviceFind(slave_info.eep_man,slave_info.eep_id)
                if(slave.hasOwnProperty("eeprom_addr")){
                    let size = 0;
                    if(slave.eeprom_addr.type == 2){
                        size = 1;
                    }
                    else if(slave.eeprom_addr.type == 4){
                        size = 2;
                    }
                    let alarm_value = ethercatmaster.writeSdo(slaveSel.currentIndex,slave.eeprom_addr.main_index,slave.eeprom_addr.sub_index,size,slave.eeprom_addr.value)
                }
                dialog.waring(qsTr("伺服EEPROM写入完成"),qsTr("伺服EEPROM写入完成"))
            }
        }

        Timer{
            repeat: true
            running: true
            interval: 100
            onTriggered: {
                if(slaveDeviceListModel.count == 0)
                    return;
                for(let i = 0;i<slaveDeviceListModel.count;i++){
                    let slave_info = slaveDeviceListModel.get(i);
                    var slave = EthercatInfoJs.ethercatSlaveDeviceFind(slave_info.eep_man,slave_info.eep_id)
                    var islost = ethercatmaster.getSlaveIslost(i);
                    slaveDeviceListModel.setProperty(i,"ec_state",ethercatmaster.getSlaveState(i))
                    if(islost){
                        if(!slave_info.islost){
                            dialog.waring(qsTr("严重错误"),qsTr("从站")+i+" "+slave_info.name+qsTr("掉线"))
                        }
                    }
                    slaveDeviceListModel.setProperty(i,"islost",islost)

                    if(slave == undefined){

                    }
                    else{
                        if(slave.hasOwnProperty("alarm_addr")){
                            let size = 0;
                            if(slave.alarm_addr.type == 2){
                                size = 1;
                            }
                            else if(slave.alarm_addr.type == 4){
                                size = 2;
                            }
                            let alarm_value = ethercatmaster.readSdo(i,slave.alarm_addr.main_index,slave.alarm_addr.sub_index,size)
                            slaveDeviceListModel.setProperty(i,"alarm_value",alarm_value)
                        }
                    }

                }

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
            text: qsTr("PDO设置")
        }
        TabButton{
            text: qsTr("运动控制")
        }
        TabButton{
            text: qsTr("从站信息")
        }

    }

    StackLayout{
        width: slaveOptionHeader.width
        height:parent.height - topHeader.height - 80
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
                    ScrollBar.vertical:ScrollBar{

                    }

                    function refreshSlaveState(){
                         slaveDeviceListModel.clear()
                        for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
                            var info = ethercatmaster.getSlaveInfo(i);
                            var slaveInfo = JSON.parse(info);
                            slaveInfo.alarm_value = 0;
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
                            Rectangle{
                                width: 1
                                height: parent.height
                                color: "black"
                            }
                            Text {
                                text: qsTr("报警码")
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignHCenter
                                width: 150
                            }
                            Rectangle{
                                width: 1
                                height: parent.height
                                color: "black"
                            }
                            Text {
                                text: qsTr("在线")
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
                            Rectangle{
                                width: 1
                                height: parent.height
                                color: "black"
                            }
                            Text {
                                text: "0x"+alarm_value.toString(16)+"("+alarm_value+")"
                                anchors.verticalCenter: parent.verticalCenter
                                horizontalAlignment: Text.AlignHCenter
                                width: 150
                            }
                            Rectangle{
                                width: 1
                                height: parent.height
                                color: "black"
                            }
                            Text {
                                text: islost==1?qsTr("掉线"):qsTr("在线")
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
                        inputWidth: 200
                        onConfigValueEditFinshed: {
                                var slaveInfo = slaveDeviceListModel.get(slaveSel.currentIndex)
                                sdoDescribe.text = sdoDescribe.sdoDescribeUpdate(slaveInfo.eep_man,slaveInfo.eep_id,mainIndex.intConfigValue(),subIndex.intConfigValue())
                        }
                    }
                    ICLineEdit{
                        id:subIndex
                        configName:qsTr("子索引")
                        isHex: true
                        height: datatype.height
                        onConfigValueEditFinshed: {
                                var slaveInfo = slaveDeviceListModel.get(slaveSel.currentIndex)
                                sdoDescribe.text = sdoDescribe.sdoDescribeUpdate(slaveInfo.eep_man,slaveInfo.eep_id,mainIndex.intConfigValue(),subIndex.intConfigValue())
                        }
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
                        inputWidth: 250
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
                Row{
                    spacing: 10
                    Text {
                        text: qsTr("描述：")
                    }
                    Text {
                        id:sdoDescribe

                        function sdoDescribeUpdate(eep_man,eep_id,sdo_main_index,sdo_sub_index){
                            var slave = EthercatInfoJs.ethercatSlaveDeviceFind(eep_man,eep_id)
                            if(slave == undefined){
                                return "";
                            }
                            var slave_sdo = slave.sdo
                            for(let i = 0;i<slave_sdo.length;i++){
                                if(slave_sdo[i].main_index == sdo_main_index && slave_sdo[i].sub_index == sdo_sub_index){
                                    return slave_sdo[i].describe;
                                }
                            }
                            return ""
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
                            width: 130
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
                            width: 130
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
                            width: 200
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }

                        Text {
                            text: qsTr("类型")
                            width: 100
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
                            width: 100
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
                            width: 600
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
                    property bool isEdit: false
                    color: (slaveObjectDictionaryList.currentIndex == index)?"gray":(index%2==0?"white":"#f5f5f5")
                    Row{
                        width: parent.width
                        height: parent.height
                        Text {
                            text: "0x"+main_index.toString(16)
                            width: 130
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: sub_index+"("+"0x"+sub_index.toString(16)+")"
                            width: 130
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            id:sdo_value
                            text: value+"("+"0x"+value.toString(16)+")"
                            width: 200
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: {
                                if(type == 1){
                                    return "int8"
                                }
                                else if(type == 2){
                                    return "int16"
                                }
                                else if(type == 4){
                                    return "int32"
                                }
                            }
                            width: 100
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: unit
                            width: 100
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: describe
                            width: 600
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignLeft
                        }
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            if(slaveObjectDictionaryList.currentIndex != index){
                                slaveObjectDictionaryList.currentItem.isEdit = false
                            }
                            slaveObjectDictionaryList.currentIndex = index;
                        }
                        onDoubleClicked: {
                            sdo__write_value.configValue = value;
                            parent.isEdit = true;
                            sdo__write_value.focus = true
                        }
                    }
                    ICLineEdit{
                        id:sdo__write_value
                        inputWidth: 200
                        anchors.verticalCenter: parent.verticalCenter
                        visible: parent.isEdit
                        x:262
                        onConfigValueEditFinshed:{
                            parent.isEdit = false

                            ///< 写sdo
                            let size = 0;
                            if(type == 2){
                                size = 1;
                            }
                            else if(type == 4){
                                size = 2;
                            }
                            ethercatmaster.writeSdo(slaveSel.currentIndex,main_index,sub_index,size,sdo__write_value.intConfigValue())
                            let  value = ethercatmaster.readSdo(slaveSel.currentIndex,main_index,sub_index,size)
                            slaveObjectDictionaryListModel.setProperty(index,"value",value);
                        }
                    }
                }

                ScrollBar.vertical:ScrollBar{

                }
                Keys.onPressed: {
                    if (event.key == Qt.Key_Up) {
                        if(slaveObjectDictionaryList.currentIndex > 0)
                            slaveObjectDictionaryList.currentIndex--;
                    }
                    else if (event.key == Qt.Key_Down) {
                        if(slaveObjectDictionaryList.currentIndex < slaveObjectDictionaryListModel.count)
                            slaveObjectDictionaryList.currentIndex++;
                    }
                    event.accepted = true;
                }
                onVisibleChanged: {
                    if(visible){
                        focus = true;
                    }
                    else{
                        focus = false;
                    }
                }
            }
        }
        Item {      //< PDO设置

        }

        Item {      //< 运动控制
            Row{
                Rectangle{
                    width: parent.parent.width / 2
                    height: parent.parent.height
                    border.width: 1
                    border.color: "black"
                    Column{
                        x:10
                        y:10
                        spacing: 10
                        Text {
                            text: qsTr("轴0")
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("指令脉冲：")
                            }
                            Text {
                                id: servo1_cmdpos
                            }
                            Text {
                                text: qsTr("Pulse")
                            }
                        }

                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("反馈脉冲：")
                            }
                            Text {
                                id: servo1_pos
                            }
                            Text {
                                text: qsTr("Pulse")
                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("实际转速：")
                            }
                            Text {
                                id: servo1_velocity
                            }
                            Text {
                                id:axis_1_control_velocity_unit
//                                text: qsTr("Rpm/min")
                            }
                        }

                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("实际扭矩：")
                            }
                            Text {
                                id: servo1_torque
                            }
                            Text {
                                text: qsTr("%")
                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("使能状态：")
                                anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle {
                                id: servo1_on
                                width: 100
                                height: 30

                                property bool servo_on_state: false

                                border.width: 1
                                border.color: "gray"

                                color: servo_on_state?"lime":"gray"
                                Text {
                                    text: servo1_on.servo_on_state?qsTr("使能"):qsTr("失能")
                                    anchors.centerIn: parent
                                }

                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        ethercatmaster.servoOn(slaveSel.currentIndex,0,!ethercatmaster.getServoOn(slaveSel.currentIndex,0));
                                    }
                                }

                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("报警：")
                            }
                            Text {
                                id: servo1_alarm
                            }
                        }
                        Row{
                            spacing: 5
                            ICLineEdit{
                                id:servo1_speed
                                configName: qsTr("速度")
                                unitText: "pules/ms"
                                configValue: "50"
                            }
                        }
                        Row{
                            spacing: 5
                            ICLineEdit{
                                id:servo1_targetPos
                                configName: qsTr("位置")
                                unitText: "pules"
                                configValue: "0"
                            }

                        }

                        Row{
                            spacing: 5
                            Button{
                                text: qsTr("正转")
                                onClicked: {
                                    ethercatmaster.setAxisJog(slaveSel.currentIndex,0,servo1_speed.intConfigValue())
                                }
                            }
                            Button{
                                text: qsTr("反转")
                                onClicked: {
                                    ethercatmaster.setAxisJog(slaveSel.currentIndex,0,-servo1_speed.intConfigValue())
                                }
                            }

                            Button{
                                text: qsTr("启动")
                            }
                            Button{
                                text: qsTr("停止")
                                onClicked: {
                                    ethercatmaster.setAxisStop(slaveSel.currentIndex,0)
                                }
                            }
                        }
                        Row{
                            spacing: 5
                            Button{
                                text: qsTr("使能")
                                onClicked: {
                                    ethercatmaster.servoOn(slaveSel.currentIndex,0,true);
                                }
                            }
                            Button{
                                text: qsTr("失能")
                                onClicked: {
                                    ethercatmaster.servoOn(slaveSel.currentIndex,0,false);
                                }
                            }
                            Button{
                                text: qsTr("清除报警")
                                onClicked: {
                                    ethercatmaster.clearServoAlarm(slaveSel.currentIndex,0);
                                }
                            }
                        }
                    }


                    Timer{
                        running: visible
                        interval: 50
                        repeat: true
                        onTriggered: {
                            if(slaveSel.currentIndex == -1)
                                return
                            servo1_pos.text = ethercatmaster.getServoPos(slaveSel.currentIndex,0)
                            let alarm_num = ethercatmaster.getServoAlarm(slaveSel.currentIndex,0)
                            let slave_info = slaveDeviceListModel.get(slaveSel.currentIndex);
                            let slave = EthercatInfoJs.ethercatSlaveDeviceFind(slave_info.eep_man,slave_info.eep_id)
                            if(alarm_num){
                                servo1_alarm.text  = alarm_num
                                try{
                                    if(slave.hasOwnProperty("alarm_list")){
                                        servo1_alarm.text +=":"+slave.alarm_list[alarm_num]
                                    }
                                }
                                catch(err){

                                }

                            }
                            else{
                                servo1_alarm.text = qsTr("无")
                            }
                            servo1_on.servo_on_state =  ethercatmaster.getServoOn(slaveSel.currentIndex,0)
                            servo1_cmdpos.text = ethercatmaster.getServoCmdPos(slaveSel.currentIndex,0)
                            servo1_velocity.text = ethercatmaster.getServoVelocity(slaveSel.currentIndex,0)
                            servo1_torque.text = ethercatmaster.getServoTorque(slaveSel.currentIndex,0)

                        }
                    }
                }
                Rectangle{
                    id:axis_2_control_page
                    width: parent.parent.width / 2
                    height: parent.parent.height
                    border.width: 1
                    border.color: "black"
                    Column{
                        x:10
                        y:10
                        spacing: 10
                        Text {
                            text: qsTr("轴1")
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("指令脉冲：")
                            }
                            Text {
                                id: servo2_cmdpos
                            }
                            Text {
                                text: qsTr("Pulse")
                            }
                        }

                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("反馈脉冲：")
                            }
                            Text {
                                id: servo2_pos
                            }
                            Text {
                                text: qsTr("Pulse")
                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("实际转速：")
                            }
                            Text {
                                id: servo2_velocity
                            }
                            Text {
                                text: axis_1_control_velocity_unit.text
                            }
                        }

                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("实际扭矩：")
                            }
                            Text {
                                id: servo2_torque
                            }
                            Text {
                                text: qsTr("%")
                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("使能状态：")
                                anchors.verticalCenter: parent.verticalCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                            Rectangle {
                                id: servo2_on
                                width: 100
                                height: 30

                                property bool servo_on_state: false

                                border.width: 1
                                border.color: "gray"

                                color: servo_on_state?"lime":"gray"
                                Text {
                                    text: servo2_on.servo_on_state?qsTr("使能"):qsTr("失能")
                                    anchors.centerIn: parent
                                }

                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        ethercatmaster.servoOn(slaveSel.currentIndex,1,!ethercatmaster.getServoOn(slaveSel.currentIndex,1));
                                    }
                                }

                            }
                        }
                        Row{
                            spacing: 5
                            Text {
                                text: qsTr("报警：")
                            }
                            Text {
                                id: servo2_alarm
                            }
                        }
                        Row{
                            spacing: 5
                            ICLineEdit{
                                id:servo2_speed
                                configName: qsTr("速度")
                                unitText: "pules/ms"
                                configValue: "50"
                            }
                        }
                        Row{
                            spacing: 5
                            ICLineEdit{
                                id:servo2_targetPos
                                configName: qsTr("位置")
                                unitText: "pules"
                                configValue: "0"
                            }

                        }

                        Row{
                            spacing: 5
                            Button{
                                text: qsTr("正转")
                                onClicked: {
                                    ethercatmaster.setAxisJog(slaveSel.currentIndex,1,servo2_speed.intConfigValue())
                                }
                            }
                            Button{
                                text: qsTr("反转")
                                onClicked: {
                                    ethercatmaster.setAxisJog(slaveSel.currentIndex,1,-servo2_speed.intConfigValue())
                                }
                            }

                            Button{
                                text: qsTr("启动")
                            }
                            Button{
                                text: qsTr("停止")
                                onClicked: {
                                    ethercatmaster.setAxisStop(slaveSel.currentIndex,1)
                                }
                            }
                        }
                        Row{
                            spacing: 5
                            Button{
                                text: qsTr("使能")
                                onClicked: {
                                    ethercatmaster.servoOn(slaveSel.currentIndex,1,true);
                                }
                            }
                            Button{
                                text: qsTr("失能")
                                onClicked: {
                                    ethercatmaster.servoOn(slaveSel.currentIndex,1,false);
                                }
                            }
                            Button{
                                text: qsTr("清除报警")
                                onClicked: {
                                    ethercatmaster.clearServoAlarm(slaveSel.currentIndex,1);
                                }
                            }
                        }
                    }


                    Timer{
                        running: visible
                        interval: 50
                        repeat: true
                        onTriggered: {
                            if(slaveSel.currentIndex == -1)
                                return
                            servo2_pos.text = ethercatmaster.getServoPos(slaveSel.currentIndex,1)
                            let alarm_num = ethercatmaster.getServoAlarm(slaveSel.currentIndex,1)
                            let slave_info = slaveDeviceListModel.get(slaveSel.currentIndex);
                            let slave = EthercatInfoJs.ethercatSlaveDeviceFind(slave_info.eep_man,slave_info.eep_id)
                            if(alarm_num){
                                servo2_alarm.text  = alarm_num
                                try{
                                    if(slave.hasOwnProperty("alarm_list")){
                                        servo2_alarm.text +=":"+slave.alarm_list[alarm_num]
                                    }
                                }
                                catch(err){

                                }

                            }
                            else{
                                servo2_alarm.text = qsTr("无")
                            }
                            servo2_on.servo_on_state =  ethercatmaster.getServoOn(slaveSel.currentIndex,1)
                            servo2_cmdpos.text = ethercatmaster.getServoCmdPos(slaveSel.currentIndex,1)
                            servo2_velocity.text = ethercatmaster.getServoVelocity(slaveSel.currentIndex,1)
                            servo2_torque.text = ethercatmaster.getServoTorque(slaveSel.currentIndex,1)

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
                Row{
                    Text {
                        id:slaveAllInfo
                        width: 400
                    }
                    Rectangle{
                        width: 400
                        height: 500
                        border.width: 1
                        border.color: "black"
                        Text {
                            id: slaveSiiInfoName
                            text: qsTr("SII 信息:")
                            x:10
                            y:10
                        }
                        Text {
                            id: slaveSiiInfo
                            anchors.top:slaveSiiInfoName.bottom
                            anchors.left: slaveSiiInfoName.left
                            anchors.topMargin: 10
                        }
                    }

                }

            }

        }
        onCurrentIndexChanged: {

        }
    }

    FileDialog{
        id: openFileDialog
        title: qsTr("伺服参数文件位置")
        nameFilters: [ "ESD files(*.esd)","所有文件(*)"]
        onAccepted: {
            var esd = ethercatmaster.readFile(fileUrl);
            esdfileListModel.clear()
            if(esd.length == 0)
                return;
            var esd_ = JSON.parse(esd);
            var esd_sdo = esd_.sdo;
            for(let i = 0;i<esd_sdo.length;i++){
                esdfileListModel.append(esd_sdo[i]);
            }
            esdfilePage.visible = true;
            esdfilePage.title = fileUrl;
            esdfileList.eep_id = esd_.eep_id
            esdfileList.eep_man = esd_.eep_man
        }
    }
    FileDialog{
        id: outputFileDialog
        title: qsTr("伺服参数文件保存位置")
        nameFilters: ["ESD files(*.esd)","所有文件(*)"]
        selectMultiple: false
        selectExisting:false
        onAccepted: {
            if(slaveSel.currentIndex == -1){
                return
            }
            var slaveInfo = slaveDeviceListModel.get(slaveSel.currentIndex)
            var info = EthercatInfoJs.ethercatSlaveDeviceFind(slaveInfo.eep_man,slaveInfo.eep_id);
            for(let i = 0;i<slaveObjectDictionaryListModel.count;i++){
                info.sdo[i].value = slaveObjectDictionaryListModel.get(i).value;
            }
            ethercatmaster.writeFile(fileUrl,JSON.stringify(info,null,2))
            dialog.waring(qsTr("伺服参数导出完成"),qsTr("伺服参数导出完成"))
        }
        Component.onCompleted: {

        }
    }

    FileDialog{
        id: openFileDialog_alias
        title: qsTr("伺服SII文件位置")
        nameFilters: [ "EII files(*.eii)","所有文件(*)"]

        onAccepted: {
            siiDeviceListModel.clear()
            siiPage.title = qsTr("SII ")+fileUrl
            var esi = ethercatmaster.readFile(fileUrl);
            if(esi.length == 0){
                return;
            }
            var esi_ = JSON.parse(esi);

            for(let i = 0;i<esi_.length;i++){
                siiDeviceListModel.append(esi_[i])
            }
            siiPage.visible = true
        }
    }
    FileDialog{
        id: outputFileDialog_alias
        title: qsTr("伺服SII文件保存位置")
        nameFilters: ["EII files(*.eii)","所有文件(*)"]
        selectMultiple: false
        selectExisting:false
        onAccepted: {
            var info=[]
            for(var i = 0;i<slaveDeviceListModel.count;i++){
                let slaveInfo = slaveDeviceListModel.get(i)
                info.push(slaveInfo)
            }
            ethercatmaster.writeFile(fileUrl,JSON.stringify(info,null,2))
            dialog.waring(qsTr("伺服SII文件导出完成"),qsTr("伺服SII文件导出完成"))
        }
        Component.onCompleted: {

        }
    }
    FileDialog{
        id: openFileDialog_firmware
        title: qsTr("伺服升级文件位置")
        nameFilters: ["更新包(*.tar.bfe)","固件文件(*.bin)","所有文件(*)"]

        onAccepted: {
            firmwareFilePath.text = fileUrl
            firmwarePage.visible = true
        }
    }

    Window{
        id:firmwarePage
        width: 800
        height: 600
        visible: false;
        Column{
            spacing: 10
            x:10
            y:10
            Row{
                spacing: 10
                Text {
                    text: qsTr("升级文件位置：")
                }
                Text{
                    id:firmwareFilePath
                }
            }

            Row{
                spacing: 10
                visible: false
                Text{
                    text:qsTr("选择从站：")
                    anchors.verticalCenter: parent.verticalCenter
                    verticalAlignment: Text.AlignVCenter
                }
                ComboBox{
                    id:firmwareSlaveSel
                }

            }
            ListView{
                id:firmwareSlaveList
                model:ListModel{
                    id:firmwareSlaveListModel
                }
                ScrollBar.vertical:ScrollBar{

                }

                width: parent.width
                height: 400
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
                            text: qsTr("是否升级")
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
                            text: qsTr("从站")
                            width: 250
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: qsTr("状态")
                            width: 200
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
                    property bool isEdit: false
                    color: (firmwareSlaveList.currentIndex == index)?"gray":(index%2==0?"white":"#f5f5f5")
                    Row{
                        width: parent.width
                        height: parent.height
                        CheckBox{
                            checked: isUpdate==1?true:false
                            onClicked: {
                                firmwareSlaveListModel.setProperty(index,"isUpdate",checked?1:0)
                            }
                            width: 80
                            anchors.verticalCenter: parent.verticalCenter
                            height: parent.height
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text:index+"   "+aliasadr+":"+SIIindex+" "+name
                            width: 250
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                        Rectangle{
                            width: 1
                            height: parent.height
                            color: "black"
                        }
                        Text {
                            text: updateState==-1?qsTr("未开始"):(updateState==0?qsTr("等待中"):(updateState==100?qsTr("升级完成"):(qsTr("升级中:")+updateState+"%")))
                            width: 200
                            anchors.verticalCenter: parent.verticalCenter
                            horizontalAlignment: Text.AlignHCenter
                        }
                    }
                }
            }
            Button{
                text:qsTr("开始升级")
                onClicked: {
//                    if(firmwareSlaveSel.currentIndex == -1){
//                        return;
//                    }
                    firmwareSlaveTimer.start()
                    var binPath = ethercatmaster.unTarBfeFile(firmwareFilePath.text)
                    for(var i = 0;i<firmwareSlaveListModel.count;i++){
                        if(firmwareSlaveListModel.get(i).isUpdate){
                            firmwareSlaveTimer.currentSlave = i;
                            ethercatmaster.updateSlaveFirm(i,binPath)
                            firmwareSlaveListModel.setProperty(i,"updateState",100)
                        }
                    }
                    dialog.waring(qsTr("伺服升级"),qsTr("伺服升级完成"))
                    firmwareSlaveTimer.stop()
                }
            }
        }
        Timer{
            id:firmwareSlaveTimer
            interval: 100
            running: false
            repeat: true

            property int currentSlave: 0

            onTriggered: {
                var value = ethercatmaster.getFirmwareUpdateValue();                  //< 当前更新百分比
                firmwareSlaveListModel.setProperty(currentSlave,"updateState",value)
            }
        }
        onVisibleChanged: {
            if(visible){
                firmwareSlaveListModel.clear();
                for(let i = 0;i<ethercatmaster.getSlaveCount();i++){
                    var info = ethercatmaster.getSlaveInfo(i);
                    var slaveInfo = JSON.parse(info);
                    slaveInfo.alarm_value = 0;
                    slaveInfo.isUpdate = 0;
                    slaveInfo.updateState=-1;
                    firmwareSlaveListModel.append(slaveInfo)
                }
            }
        }
    }

    Window{
        id:esdfilePage
        visible: false
        width: 1280
        height: 700
        Row{
            id:esdOptionHeader
            spacing: 10
            ComboBox{
                id:esdSlaveSel
                width: 300
            }
            Button{
                text:qsTr("写参数")
                onClicked: {
                    if(esdSlaveSel.currentIndex == -1){
                        return ;
                    }
                    var slave_info = slaveDeviceListModel.get(esdSlaveSel.currentIndex)
                    if(esdfileList.eep_man != slave_info.eep_man){
                        dialog.waring(qsTr("伺服参数写入错误"),qsTr("伺服参数配置厂家与目标驱动器厂家不一致！"))
                        return;
                    }

                    for(let i = 0;i<esdfileListModel.count;i++){
                        let esd_sdo = esdfileListModel.get(i);

                        ///< 写sdo
                        let size = 0;
                        if(esd_sdo.type == 2){
                            size = 1;
                        }
                        else if(esd_sdo.type == 4){
                            size = 2;
                        }
                        ethercatmaster.writeSdo(esdSlaveSel.currentIndex,esd_sdo.main_index,esd_sdo.sub_index,size,esd_sdo.value)
                    }
                    dialog.waring(qsTr("伺服参数写入完成"),qsTr("伺服参数写入完成"))
                }
            }
            Button{
                text:qsTr("写EEPROM")
                onClicked: {
                    if(esdSlaveSel.currentIndex == -1){
                        return;
                    }
                    let slave_info = slaveDeviceListModel.get(esdSlaveSel.currentIndex);
                    var slave = EthercatInfoJs.ethercatSlaveDeviceFind(slave_info.eep_man,slave_info.eep_id)
                    if(slave.hasOwnProperty("eeprom_addr")){
                        let size = 0;
                        if(slave.eeprom_addr.type == 2){
                            size = 1;
                        }
                        else if(slave.eeprom_addr.type == 4){
                            size = 2;
                        }
                        let alarm_value = ethercatmaster.writeSdo(esdSlaveSel.currentIndex,slave.eeprom_addr.main_index,slave.eeprom_addr.sub_index,size,slave.eeprom_addr.value)
                    }
                    dialog.waring(qsTr("伺服EEPROM写入完成"),qsTr("伺服EEPROM写入完成"))
                }
            }
        }
        ListView{
            id:esdfileList
            model: ListModel{
                id:esdfileListModel
            }
            width: parent.width
            height: parent.height - 60
            anchors.top: esdOptionHeader.bottom
            anchors.topMargin:10
            clip: true
            ScrollBar.vertical:ScrollBar{

            }
            property int eep_man: 0
            property int eep_id: 0
            header: Rectangle{
                width: parent.width
                height: 30
                border.color: "black"
                border.width: 1
                Row{
                    width: parent.width
                    height: parent.height
                    Text {
                        text: qsTr("索引")
                        width: 130
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("子索引")
                        width: 130
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("值")
                        width: 200
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("单位")
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("类型")
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: qsTr("描述")
                        width: 600
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
                property bool isEdit: false
                color: (slaveObjectDictionaryList.currentIndex == index)?"gray":(index%2==0?"white":"#f5f5f5")
                Row{
                    width: parent.width
                    height: parent.height
                    Text {
                        text: "0x"+main_index.toString(16)
                        width: 130
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: sub_index+"("+"0x"+sub_index.toString(16)+")"
                        width: 130
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: value+"("+"0x"+value.toString(16)+")"
                        width: 200
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: {
                            if(type == 1){
                                return "int8"
                            }
                            else if(type == 2){
                                return "int16"
                            }
                            else if(type == 4){
                                return "int32"
                            }
                        }
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: unit
                        width: 100
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignHCenter
                    }
                    Rectangle{
                        width: 1
                        height: parent.height
                        color: "black"
                    }
                    Text {
                        text: describe
                        width: 600
                        anchors.verticalCenter: parent.verticalCenter
                        horizontalAlignment: Text.AlignLeft
                    }
                }
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        slaveObjectDictionaryList.currentIndex = index;
                    }
                }
            }
        }
    }

    Window{
        id:siiPage
        width: 800
        height: 600
        visible: false;
        Row{
            id:siiHeader
            spacing: 10
            ComboBox{
                id:siiSlaveSel
                width: 300
            }
            Button{
                text: qsTr("单轴写SII")
                onClicked: {
                    if(siiSlaveSel.currentIndex == -1){
                        return;
                    }
                    var sii_ = siiDeviceListModel.get(siiDeviceList.currentIndex);
                    ethercatmaster.writeAlias(siiSlaveSel.currentIndex,sii_.aliasadr);
                    dialog.waring("SII写入成功",qsTr("SII写入完成"))
                }
            }
            Button{
                text: qsTr("全部写SII")
                onClicked: {
                    var err = qsTr("从站 ")
                    if(siiDeviceListModel.count != slaveDeviceListModel.count){
                        dialog.waring("SII写入错误",qsTr("当前从站个数与SII文件中从站个数不一致"))
                        return;
                    }
                    for(let i = 0;i<siiDeviceListModel.count;i++){
                        var sii_ = siiDeviceListModel.get(i);
                        ethercatmaster.writeAlias(i,sii_.aliasadr);
                    }
                    dialog.waring("SII写入成功",qsTr("SII写入完成"))
                }
            }
        }
        Rectangle{
            id:siiDeviceListPage
            height: parent.height - siiHeader.height - 10
            width:  parent.width
            border.color: "gray"
            border.width: 1
            anchors.top:siiHeader.bottom
            anchors.topMargin: 10

            ListView{
                id:siiDeviceList
                model: ListModel{
                    id:siiDeviceListModel
                }
                width: parent.width
                height: parent.height

                clip: true
                ScrollBar.vertical:ScrollBar{

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
                    }
                }
                delegate: Rectangle{
                    width: parent.width
                    height: 30
                    border.color: "black"
                    border.width: 1
                    color: siiDeviceList.currentIndex == index?"gray":"white"
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
                    }
                    MouseArea{
                        anchors.fill: parent
                        onClicked: {
                            siiDeviceList.currentIndex = index;
                        }
                    }
                }
            }
        }
    }

    Dialog{
        id:dialog
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
        EthercatInfoJs.init();
        var list = ethercatmaster.scanNetwork();
        networkList.model = list
        for(let i =0;i<list.length;i++){
            if(list[i].indexOf("Realtek") != -1){
                networkList.currentIndex = i;
                break;
            }
        }

    }
}
