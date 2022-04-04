import QtQuick 2.0
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.12
import QtQml 2.12


ColumnLayout{
    id: tipWindows
    property int maxCount: 5
    property variant typeList: []
    property variant tipList: []
    function appendTip(type,tip){

        if(tipWindows.tipList.length>=tipWindows.maxCount){
            tipWindows.popTip();
            //return;
        }

        var typeList = tipWindows.typeList;
        typeList.unshift(type);
        tipWindows.typeList = typeList;

        var tipList = tipWindows.tipList;
        tipList.unshift(tip);
        tipWindows.tipList = tipList;

    }
    function popTip(){
        var typeList = tipWindows.typeList;
        typeList.pop();
        tipWindows.typeList = typeList;

        var tipList = tipWindows.tipList;
        var ret = tipList.pop();
        tipWindows.tipList = tipList;
        return ret;
    }
    spacing: 5
    Repeater{
        model: tipWindows.tipList.length
        Rectangle{
            Layout.alignment: Qt.AlignBottom
            width: 180
            height: 32
            opacity: (tipWindows.tipList.length-index)/tipWindows.tipList.length
            color: "#f5f5f5"
            //radius: 5
            Row{
                spacing: 5
                Rectangle{
                    width: 32
                    height: 32
                    color: "transparent"
                    Image {
                        anchors.centerIn: parent
                        width: 24
                        height: 24
                        source: tipWindows.typeList[index]===0?"/image/true1.png":"/image/error1.png"
                    }
                }

                Label{
                    id: label
                    height: 32
                    //width: 180-32
                    text: tipWindows.tipList[index]
                    verticalAlignment: Text.AlignVCenter
                }
            }

            Rectangle{
                anchors.bottom: parent.bottom
                height: 3
                //width: 200
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#ffffff" }
                    GradientStop { position: 1.0; color: "#c4c4c4" }
                }
            }


            MouseArea{
                id: m_mouseArea
                anchors.fill: parent
                hoverEnabled: true
                onContainsMouseChanged: tipTimer.running = !m_mouseArea.containsMouse;
                onClicked: {
                    if(tipWindows.tipList.length>=tipWindows.maxCount)return;
                }
            }

        }
    }
    Timer{
        id: tipTimer
        interval: (tipWindows.maxCount-tipWindows.tipList.length)*300+500
        running: true; repeat: true;
        onTriggered: {
            //if(tipWindows.tipList.length<=1)return;

            tipWindows.popTip();
        }
    }
    Component.onCompleted: {
//        appendTip(0,"保存成功!");
//        appendTip(1,"保存错误!");
//        appendTip(0,"保存成功!");
//        appendTip(1,"保存错误!");
    }
}

