import QtQuick 2.0
import QtQml 2.0
import "."

Item {
    id:root
    width:parent.width
    height: parent.height
    clip: true

    property alias title: titleName.text
    property bool autoSizeEnable: true


    Rectangle{
        id:frame
        border.color: "black"
        border.width: 1
        //color: "transparent"
        y:10
        width:parent.width-10
        height: parent.height-10
    }
    Rectangle{
        id:titleNameRec
        anchors.top: frame.top
        anchors.topMargin: -5
        anchors.left: frame.left
        anchors.leftMargin: 10
        z:10
        width: titleName.width + 6
        height: titleName.height + 6
        Text {
            id: titleName
            x:3
            y:3
        }
    }

    Component.onCompleted: {
        var item = root.children
        var count = item.length
        var maxWidth = 0;
        var maxHeight = 0;
        for(let i =0;i<count;i++){
            if(item[i] == frame || item[i] == titleNameRec){
                continue;
            }
            item[i].x += 2
            item[i].y += 25
            let itemWidth = item[i].x + item[i].width
            if(itemWidth > maxWidth){
                maxWidth = itemWidth
            }
            let itemHeight = item[i].y + item[i].height
            if(itemHeight > maxHeight){
                maxHeight = itemHeight
            }
        }
        if(autoSizeEnable){
            root.width = maxWidth +12
            root.height = maxHeight +3
        }
    }
}
