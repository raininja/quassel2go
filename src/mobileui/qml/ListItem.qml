import Qt 4.7

Item {
  property string text: ""
  property bool current: false

  width: parent.width;
  height: delegateText.height>60 ? delegateText.height * 1.1 : 60

  Rectangle {
    anchors.fill: parent
    color: "#AAAAFF"
    opacity: parent.current ? 1 : 0
  }

  Text {
    id: delegateText
    text: parent.text
    elide: Text.ElideRight;
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter
    anchors.margins: 10
  }
}

