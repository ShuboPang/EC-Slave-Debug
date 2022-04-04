import QtQuick 2.0

Item {
    width: 300
    height: 350
    Rectangle {
        width: parent.width
        height: parent.height - 100
        anchors.bottom: parent.bottom
        color:Qt.rgba(0.5,0.5,0.5,0.9)
        Grid {
            rows: 6
            columns: 6
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            flow: Grid.LeftToRight

            Repeater {
                model: 36
                ICSingleIoState{

                }
            }
        }
    }
}
