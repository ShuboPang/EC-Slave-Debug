import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls 2.5

Rectangle{
    id: root
    color: "transparent"
    height: 48
    width: 50
    property alias imageWidth: image.width
    property alias imageHeight: image.height
    property alias tootipText: toolTip.text
    property alias sourcePath: image.source
    property color hlColor: "#FFFFFF"

    signal btnClicked();

    Image {
        id: image
        anchors.centerIn: parent
        width: 32
        height: 32
        source: sourcePath
    }
    MouseArea{
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            btnClicked();
        }
        onContainsMouseChanged: {
            if(containsMouse)parent.color = hlColor;
            else parent.color = "transparent";
        }

    }
    ToolTip{
        id: toolTip
        //parent: root
        visible: mouseArea.containsMouse && text!=""
        text: tootipText
    }
}
