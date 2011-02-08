import Qt 4.7

Item {
  id: lvItem

  property string text: ""
  property string textColor: "#000000"
  property bool current: false
  property variant iconSource: ""

  width: parent.width;
  height: delegateText.height>60 ? delegateText.height * 1.1 : 60

  Rectangle {
    anchors.fill: parent
    color: "#AAAAFF"
    opacity: parent.current ? 1 : 0
  }

  Item {
    visible: parent.width <= 90
    anchors.margins: 5
    anchors.fill: parent
    anchors.horizontalCenter: parent.horizontalCenter

    Image {
      id: iconSmall
      //anchors.left: parent.left
      anchors.top: parent.top
      anchors.horizontalCenter: parent.horizontalCenter
      //anchors.margins: 10
      source: iconSource
      width: 16
      height: 16
      visible: iconSource != ""
    }

    Text {
      id: delegateTextSmall
      text: lvItem.text
      color: lvItem.textColor
      elide: Text.ElideRight
      width: parent.width
      horizontalAlignment: "AlignHCenter"
      font.pointSize: 10

//      anchors.left: parent.left
//      anchors.right: parent.right
      anchors.horizontalCenter: parent.horizontalCenter
      anchors.bottom: parent.bottom
//      anchors.margins: 10
    }
  }

  Item {
    visible: parent.width > 90
    width: parent.width
    anchors.verticalCenter: parent.verticalCenter

    Image {
      id: icon
      anchors.left: parent.left
      anchors.verticalCenter: parent.verticalCenter
      anchors.margins: 5
      source: iconSource
      width: 16
      height: 16
      visible: iconSource != ""
    }

    Text {
      id: delegateText
      text: lvItem.text
      elide: Text.ElideRight;
      font.pointSize: 10
      anchors.left: icon.visible ? icon.right : parent.left
      anchors.right: parent.right
      anchors.verticalCenter: parent.verticalCenter
      anchors.margins: 10
    }
  }

}

