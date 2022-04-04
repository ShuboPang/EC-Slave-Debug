import QtQuick 2.0

Rectangle {
    id:root
    width: list.width
    height: list.height

    property variant items: []

    signal clicked(int index)

    Column{
        id:list
        spacing: 2
        Repeater{
            model: items.length
            Text {
                id:text
                text: items[index]
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        clicked(index);
                        root.visible
                    }
                }
            }

        }
    }
}
