import QtQuick 2.12
import QtQml 2.0
import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQml.Models 2.2
import QtQuick.Controls 2.5
import QtQuick.Controls.Styles 1.4

TreeView{
    id:root
    headerVisible:false //不显示表头
    horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff //关闭横向滚动条
    frameVisible: false //背景为深色的时候  会有一条白边，false时消除边


    property Menu mouseRightMenu

    signal mouseLeftButtonClick()

    signal mouseRightButtonClick(int mouseX,int mouseY)

    signal mouseDoubleClick()

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton
        onClicked: {
            //选中当前行 -- 关键代码，来自源码里面
            var pressedRow = root.__listView.indexAt(0, mouseY + root.__listView.contentY);
            root.__listView.currentIndex = pressedRow;
            var index = parent.indexAt(mouse.x, mouse.y);
            //console.log("pressedRow:",index,pressedRow);
            //root.clicked(index);
            selection.setCurrentIndex(index,0x0010);//点击文本，选中该节点
            if(mouse.button === Qt.RightButton) {
                if (index.valid) {
                    //打开菜单
                    mouseRightButtonClick(mouse.x,mouse.y + root.y);
                    if(mouseRightMenu != undefined){
                        mouseRightMenu.x = mouse.x;
                        mouseRightMenu.y = mouse.y + root.y;
                        mouseRightMenu.popup();
                    }
                }
            }
            else{
                mouseLeftButtonClick();
            }

        }

        onDoubleClicked: {
            if(mouse.button === Qt.LeftButton) {
                var index = parent.indexAt(mouse.x, mouse.y)
                if (index.valid) {
                    if(root.isExpanded(index))
                        root.collapse(index)
                    else
                        root.expand(index)
                    mouseDoubleClick()
                }
            }
        }
    }

}
