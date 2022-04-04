import QtQuick 2.12
import QtQuick.Controls 2.6
import QtQml 2.0

Rectangle {
    id: root
    property alias configValue:input.text
    property alias nameText: name.text
    property alias configName: name.text
    property alias inputWidth: dataInput.width
    property alias textWidth: name.width
    property alias configNameWidth: name.width
    property alias unitText: unit.text
    property alias lineFocus: input.focus
    property alias echoMode: input.echoMode

    property bool isNumber: true
    property alias decimal: doubleInput.decimals
    property alias min: doubleInput.bottom
    property alias max: doubleInput.top
    property bool isHex: false


    signal configValueEditFinshed();
    signal configDataChanged();


    function intConfigValue(){
        return parseInt(input.text)
    }

    function doubleConfigValue(){
        return parseFloat(input.text)
    }
    function strConfigValue(){
        return input.text
    }
    function numConfigValue(){
        return Number(input.text)
    }



    width: name.width+dataInput.width+unit.width
    height: 28
    color: "transparent"
    DoubleValidator {
        id:doubleInput
        decimals: 3
        notation: DoubleValidator.StandardNotation
        bottom: -99999999
        top: 999999999
    }
    RegExpValidator {
        id: hex
        regExp: /0[xX][[0-9a-fA-F]]+/
    }
    Row{
        Label {
            id: name
            text: qsTr("")
            height: root.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            font.bold: input.focus
            background: Rectangle{color: "transparent"}
            font.pixelSize: (dataInput.height-10)
        }
        Rectangle {
            id:dataInput
            width: 100
            height: root.height
            border.color: input.focus?"#1E90FF":"black"
            border.width: input.focus?2:1
            clip:true
            TextInput {//TextInput
                id:input
                width: dataInput.width-10
                height: dataInput.height-10
                font.pixelSize: (dataInput.height-10)
                echoMode: TextInput.Normal
//                focus: true
//                anchors.verticalCenter: parent.verticalCenter
//                horizontalAlignment: TextInput.Left
//                topPadding: (dataInput.height-font.pointSize)/4 + 1
//                bottomPadding: topPadding
//                leftPadding:6
//                rightPadding:6
                clip: true
                anchors.centerIn: parent
                selectionColor: "#007ACC"
                selectByMouse:true
                validator:isHex?hex:isNumber?doubleInput:null
                onEditingFinished: {
                    configValueEditFinshed();
                }

                onTextEdited: {
                    configDataChanged();
                }

                onVisibleChanged: {
                    //if(!visible)focus = false;
                }
            }
        }
        Label {
            id: unit
            text: qsTr("")
            height: root.height
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            background: Rectangle{color: "transparent"}
        }
    }
}
