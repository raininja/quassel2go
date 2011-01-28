import Qt 4.7

ListView {
    id: nickListView

    model: ListModel {
        ListElement { plaintext: "1 Operator"; type: "Header" }
        ListElement { plaintext: "ChanServ"; type: "User" }
        ListElement { plaintext: "3 Voiced"; type: "Header" }
        ListElement { plaintext: "asdsdf"; type: "User" }
        ListElement { plaintext: "dddd"; type: "User" }
        ListElement { plaintext: "ddd"; type: "User" }
        ListElement { plaintext: "dff"; type: "User" }
        ListElement { plaintext: "10 Users"; type: "Header" }
        ListElement { plaintext: "asdsdf"; type: "User" }
        ListElement { plaintext: "dddd"; type: "User" }
        ListElement { plaintext: "ddd"; type: "User" }
        ListElement { plaintext: "dff"; type: "User" }
        ListElement { plaintext: "asdsdf"; type: "User" }
        ListElement { plaintext: "dddd"; type: "User" }
        ListElement { plaintext: "ddd"; type: "User" }
        ListElement { plaintext: "dff"; type: "User" }
    }

    delegate: Item {
        width: parent.width;
        height: nickListViewDelegateText.height>70 ? nickListViewDelegateText.height:70

        Rectangle {
            color: "#3333ff"
            anchors.fill: parent
            opacity: parent.ListView.isCurrentItem ? 1 : 0
        }

        Text {
            id: nickListViewDelegateText
            text: type=="Header" ? plaintext : "   "+plaintext;
            elide: Text.ElideRight;
            font.bold: type=="Header"
            anchors.verticalCenter: parent.verticalCenter
            width: parent.width
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                //if(model[index].type != "Header")
                    parent.ListView.view.currentIndex = index
            }
        }
    }
    contentY:  10000

    ScrollBar {
        scrollArea: parent
    }
}
