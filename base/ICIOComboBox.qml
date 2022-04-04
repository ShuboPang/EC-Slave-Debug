import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4
import "../IODefines.js" as IoDefinesJs

Rectangle {
    id: root
    width: 100
    height: 32
    border.width: 1
    color: mouseArea.containsMouse?"#f3f3f3":"#ffffff"
    property bool isInputBox: true
    property int currentIndex: 0

    onCurrentIndexChanged: {
        btn.text = root.currentIndex+":"+(root.isInputBox?IoDefinesJs.getIoInputAbbreviationName(root.currentIndex):
                               IoDefinesJs.getIoOutputAbbreviationName(root.currentIndex));
    }
    RowLayout{
        anchors.fill: parent
        anchors.margins: 6
        Label{
            id: btn
            width: root.width-root.height+12
            height: root.height-12
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: root.isInputBox?IoDefinesJs.getIoInputAbbreviationName(root.currentIndex):IoDefinesJs.getIoOutputAbbreviationName(root.currentIndex)
            Popup{
                id: popup
                y: parent.height
                x: -(popup.width-root.width)/2-50
                height: 200
                width: 395
                TabView{
                    id: tabview
                    anchors.fill: parent
                    style: TabViewStyle {
                          tab: Rectangle {
                              color: !styleData.selected ? "#ffffff" : "#78B5DE"
                              border.color: !styleData.selected ? "#f5f5f5" : "#78B5DE"
                              border.width: 1
                              implicitWidth: tabview.width/4
                              implicitHeight: 26
                              radius: 3
                              Text {
                                  id: text
                                  anchors.centerIn: parent
                                  text: styleData.title
                                  color: !styleData.selected ? "#000000" : "#ffffff"
                                  font.bold: styleData.selected
                              }
                          }
                      }
                    Repeater{
                        model: 4
                        Tab{
                            id: repeaterTab
                            title: index
                            property int currentIndex: index
                            Rectangle {
                                Grid{
                                    id:actionMenu
                                    columns: 8
                                    x:3
                                    y:5
                                    spacing: 4
                                    Repeater{
                                        model: (root.isInputBox?IoDefinesJs.xDefines.length:IoDefinesJs.yDefines.length)/4
                                        Label{
                                            width: 42
                                            height: 26
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            text: root.isInputBox?IoDefinesJs.getIoInputAbbreviationName(index+(repeaterTab.currentIndex*IoDefinesJs.xDefines.length/4)).split(":")[0]:
                                                            IoDefinesJs.getIoOutputAbbreviationName(index+(repeaterTab.currentIndex*IoDefinesJs.yDefines.length/4)).split(":")[0]
                                            background: Rectangle{color: subMouseArea.containsMouse?"#c4c4c4":"#f5f5f5"}
                                            color: subMouseArea.containsMouse?"red":"#000"
                                            MouseArea{
                                                id: subMouseArea
                                                anchors.fill: parent
                                                hoverEnabled: true
                                                onClicked: {
                                                    //var i = index+(repeaterTab.currentIndex*IoDefinesJs.xDefines.length/4);
                                                    root.currentIndex = index+(repeaterTab.currentIndex*(root.isInputBox?IoDefinesJs.xDefines.length:IoDefinesJs.yDefines.length)/4);
                                                    btn.text = (root.isInputBox?IoDefinesJs.getIoInputAbbreviationName(index+(repeaterTab.currentIndex*IoDefinesJs.xDefines.length/4)):
                                                                           IoDefinesJs.getIoOutputAbbreviationName(index+(repeaterTab.currentIndex*IoDefinesJs.yDefines.length/4)));
                                                    popup.close();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

            }
        }

        Rectangle{
            //anchors.right: parent.right
            Layout.alignment: Qt.AlignRight
            width: root.height-12
            height: root.height-12
            color: "transparent"
            Image {
                anchors.centerIn: parent
                width: 16
                height: 16
                source: "/image/combobox1.png"
            }
        }

    }

    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            if(popup.visible)
                popup.close();
            else
                popup.open();
        }
    }
}
