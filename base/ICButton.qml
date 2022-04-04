import QtQuick 2.0
import QtQml 2.12
Rectangle {
    id:container
    property alias text: text.text
    property alias icon: icon.source
    property int iconPos: -1
    property int customIconX: 0
    property int iY: 0
    property int customTextX: 0
    property int customTextY: 0
    property bool isAutoRepeat: false
    property int autoInterval: 50
    property int delayOnAutoRepeat: 200
    property string bgColor: "white"
    property string bdColor: "grey"
    property alias textColor: text.color
    property alias font: text.font
    property alias mousePosX:mouse.mouseX
    property alias mousePosY:mouse.mouseY
    property int bWidth: 100
    property int bheight:32
    property bool isUsePrsAHold: false
    property alias timer: autoTimer
    property bool borderVisible: true
    property bool isPreventStealing: true
    property alias iconWidth: icon.width
    property alias iconHeight: icon.height
    property alias isContainsMouse: mouse.containsMouse
    Drag.active: mouse.drag.active;
    Drag.supportedActions: Qt.CopyAction;
    Drag.dragType: Drag.Automatic;

    signal buttonClicked()
    signal clickedText(string text)
    signal triggered()
    signal btnPressed()
    signal btnReleased()
    signal btnPressHold();

    function clicked(){
        buttonClicked();
    }
    function pressed(){
        btnPressed();
    }
    function released(){
        btnReleased();
    }

    width: bWidth
    height: bheight
    border.width: borderVisible?1:0
    border.color: bdColor
    color: bgColor

    onBgColorChanged: {
        color = bgColor;
    }

    state: enabled ? "" : "disabled"

    states: [
        State {
            name: "disabled"
            PropertyChanges { target: container; color:"gray";}
            PropertyChanges { target: text; color:"gainsboro";}

        }

    ]
    Image {
        id: icon
        fillMode: Image.Stretch
        onProgressChanged: {
            if(progress == 1.0){
                if(source === "")
                    iconPos = -1;
                else{
                    if(iconPos === -1)
                        iconPos = 0;
                }
            }
        }

        visible: iconPos >= 0
        //            anchors.left: iconPos == 0 ? parent.left : text.right
    }
    Text {
        id: text
        text: ""
        y: (parent.height - height) >> 1
        x: (parent.width - width) >> 1
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WrapAnywhere
    }

    Component.onCompleted: {
        if(iconPos === 0){
            icon.x = 2;
            text.horizontalAlignment = Text.AlignLeft;
            text.x = icon.x + icon.paintedWidth + 2;
            text.y = (container.height - text.height) >> 1;
            icon.y = (container.height - icon.paintedHeight) >> 1;
        }else if(iconPos === 1){
            text.x = 2;
            text.y = (container.height - text.height) >> 1;
            text.horizontalAlignment = Text.AlignLeft;
            icon.x = text.x + text.width + 2;
            icon.y = (container.height - icon.paintedHeight) >> 1;
        }else if(iconPos === 2){
            text.y = 0;
            text.horizontalAlignment = Text.AlignLeft;
            icon.y = text.y + text.font.pixelSize + 2;
        }else if(iconPos === 4){
            text.x = customTextX;
            text.y = customTextY;
            icon.x = customIconX;
            icon.y = iY;
        }
    }

    MouseArea{
        id:mouse
        anchors.fill: parent
        preventStealing: false
        drag.target: parent
        hoverEnabled: true
        property int oldX: 0
        property int oldY: 0
        onPressed: {
            if(isPreventStealing)
                parent.color = "lightsteelblue";
            if(isAutoRepeat){
                triggered()
                autoTimer.interval = delayOnAutoRepeat;
                autoTimer.start();
            }
            btnPressed();
            mouse.focus = true
            mouse.oldX = parent.x
            mouse.oldY = parent.y
        }
        onReleased: {
            parent.color = bgColor;
            if(isAutoRepeat)
                autoTimer.stop();
            btnReleased()
            if(parent.Drag.supportedActions == Qt.CopyAction){
                parent.x = mouse.oldX ;
                parent.y = mouse.oldY ;
            }
        }

        onCanceled: {
            parent.color = bgColor;
            if(isAutoRepeat){
                autoTimer.stop();
            }
            btnReleased();
        }

        onExited: {
            parent.color = bgColor;
            if(isAutoRepeat){
                autoTimer.stop();
            }
            btnReleased();
        }

        onClicked: {
            buttonClicked();
            clickedText(text.text);
            if(!isAutoRepeat)
                triggered();
        }

        onPressAndHold: {
            btnPressHold();
        }

        onVisibleChanged: {
            if(!visible && pressed){
                parent.color = bgColor;
                if(isAutoRepeat){
                    autoTimer.stop();
                }
                btnReleased();
            }
        }


    }



    Timer{
        id:autoTimer
        interval: 50; running: true; repeat: true;
        onTriggered: {
            autoTimer.interval = autoInterval;
            parent.triggered();
        }
    }
}
