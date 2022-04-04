import QtQuick 2.6
import QtQuick.Controls 2.6
import QtQml 2.0

Rectangle {
    id:root
    property alias configValue:input.text
    property alias nameText: name.text
    property alias unitText: unit.text
    property alias inputWidth: dataInput.width
    property alias bgcolor: dataInput.color
    property alias getInputText: input.text
    property alias textWidth: name.width
    property alias textColor: input.color
    property string labelText: ""
    property string configKey: ""
    property int decimal: doubleInput.decimals
    property bool isNumber: true

    property bool editing: false

    width: editBox.width
    height: editBox.height
    color: "transparent"

    signal editFinish()

    function getConfigValueDouble(){
        var temp = parseFloat(input.text).toFixed(3)
        return Number(temp)
    }

    function getConfigValueInt(){
        return parseInt(input.text)
    }

    DoubleValidator {
        id:doubleInput
        decimals: 9
        notation: DoubleValidator.StandardNotation
    }


    Row{
        id:editBox
        width: dataInput.width +name.width +  unit.width+10
        height: 25
        spacing: 5

        Text {
            id:name
            font.pointSize: 10
            //anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment:Text.AlignRight
        }

        Rectangle {
            id:dataInput
            width: 100
            height: 25
            border.color: input.focus?"#1E90FF":"black"
            border.width: input.focus?2:1
            //anchors.verticalCenter: parent.verticalCenter
            clip:true
//            color: root.editing?"lime":"transparent"

            TextInput {
                id:input
                width: dataInput.width
                height: dataInput.height
                //focus: true
                horizontalAlignment: TextInput.Left
                rightPadding:3
                font.pointSize: 12
                selectByMouse:true
                validator:isNumber?doubleInput:null
                onEditingFinished: {
                    editFinish();
                    if (labelText != "") {
                        hc_yk_control.upData(label,text.toString());
                    }
                    root.editing=false
                }
                onTextChanged: {
                    root.editing=true
                }
            }
        }
        Text {
            id: unit
            font.pointSize: 13
            //anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment:Text.AlignLeft
        }
    }

    Component.onCompleted: {
        if(configKey.length){
            configValue = hc_yk_control.loadData(configKey,"0");
        }
    }
}
