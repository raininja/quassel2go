import Qt 4.7

Item {
  property string text: ""

  height: bufferlistViewDelegateText.height * 1.2

  Rectangle {
    anchors.fill: parent
    color: "#AAAAAA"
    //opacity: 1 // parent.ListView.isCurrentItem ? 1 : 0
  }

  Text {
    id: bufferlistViewDelegateText
    text: parent.text
    elide: Text.ElideRight;
    font.bold: true // type=="Network"
    anchors.left: parent.left
    anchors.right: parent.right
    anchors.verticalCenter: parent.verticalCenter
    anchors.margins: 10
  }
}

