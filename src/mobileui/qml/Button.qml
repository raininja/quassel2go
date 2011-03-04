import Qt 4.7

Item {
    id: container

    signal clicked

    property string text
    property bool keyUsing: false

    property bool checked: false

    property variant iconSource: ""

    width: btnText.width+2*rect.radius + (icon.visible ? (5 + icon.width) : 0)
    height: btnText.height * 1.5

    Rectangle {
        id: rect
        anchors.fill: parent
        radius: container.height*1/5
        anchors.margins: 4
        border.color: "#000000"
        border.width: 2
        color: container.state == "Pressed" ? "#555555" : "#888888"
    }

    MouseArea {
        id: mouseRegion
        anchors.fill: rect
        onClicked: { container.clicked(); }
    }

    Image {
      id: icon
      anchors.left: parent.left
      anchors.verticalCenter: parent.verticalCenter
      anchors.margins: 5

      source: iconSource
      width: 48
      height: 48
      visible: iconSource != ""
    }

    Text {
        id: btnText
        color: if(container.keyUsing){"#D0D0D0";} else {"#FFFFFF";}
        anchors.left: icon.visible ? icon.right : parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.leftMargin: icon.visible ? 5 : anchors.margins

        horizontalAlignment: "AlignHCenter"
        verticalAlignment: "AlignVCenter"
        font.bold: true
        text: container.text; /*style: Text.Raised;*/ styleColor: "black"
        //font.pixelSize: 12
    }
    states: [
        State {
            name: "Pressed"
            when: mouseRegion.pressed == true || container.checked == true
            // PropertyChanges { target: pressed; opacity: 1 }
        },
        State {
            name: "Focused"
            when: container.activeFocus == true
            PropertyChanges { target: btnText; color: "#FFFFFF" }
        }
    ]
    transitions: Transition {
        ColorAnimation { target: btnText; }
    }
}
