import Qt 4.7

Item {
  id: statusWidget
  property string text: ""
  width: 300
  height: textItem.height

  Rectangle {
    anchors.fill: parent
    color: "white"
    radius: 10
    opacity: 0.6
  }

  Text {
    id: textItem
    text: parent.text
    anchors.margins: 10
    anchors.fill: parent
  }

  MouseArea {
    anchors.fill: parent
    onClicked: {
      parent.visible = false
    }
  }
}
