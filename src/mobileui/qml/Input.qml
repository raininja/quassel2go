import Qt 4.7

FocusScope {
    id:container
    width: 220
    height: 28

    signal accepted
    property alias text: input.text
    property alias item:input

    Rectangle {
        id: rect
        anchors.fill: parent
        anchors.margins: 4
        radius: container.height*1/5
        border.color: "#000000"
        border.width: 2
        color: container.focus ? "#aaaaaa" : "#eeeeee"
    }

    TextInput {
        id: input
        width: parent.width - 30
        anchors.centerIn: parent
        color: "#151515";
        focus: true
        onAccepted:{container.accepted()}
        text: ""
        selectByMouse: true
    }
}
