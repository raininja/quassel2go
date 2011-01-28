import Qt 4.7

ListView {
    id: bufferlistView

    delegate: Item {
        width: parent.width;
        height: bufferlistViewDelegateText.height>70 ? bufferlistViewDelegateText.height:70

        Rectangle {
            color: "#3333ff"
            anchors.fill: parent
            opacity: parent.ListView.isCurrentItem ? 1 : 0
        }

        Text {
            id: bufferlistViewDelegateText
            text: type=="Network" ? plaintext : "   "+plaintext;
            elide: Text.ElideRight;
            font.bold: type=="Network"
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                parent.ListView.view.currentIndex = index
            }
        }
    }
    contentY:  10000

    ScrollBar {
        scrollArea: parent
    }
}
