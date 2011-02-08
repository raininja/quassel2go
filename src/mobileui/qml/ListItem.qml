import Qt 4.7

Item {
  id: lvItem

  property string text: ""
  property string textColor: "#000000"
  property bool current: false
  property variant iconSource: ""

  property bool header: false

  width: parent.width;
  height: (lvItem.header&&vertItem.visible) ? delegateText.height * 1.2 : (delegateText.height>60 ? delegateText.height * 1.2 : 60)

  Rectangle {
    anchors.fill: parent
    color: lvItem.header ? "#AAAAAA" : "#AAAAFF"
    opacity: (vertItem.visible&&lvItem.header) ? 1 : (parent.current ? 1 : 0)
  }

  Item {
    id: horItem
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

      font.bold: lvItem.header
    }
  }

  Item {
    id: vertItem
    visible: parent.width > 90
    width: parent.width
    anchors.verticalCenter: parent.verticalCenter

    Image {
      id: icon
      anchors.left: parent.left
      anchors.verticalCenter: parent.verticalCenter
      anchors.margins: 5

      anchors.leftMargin: lvItem.header ? 5 : 15

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

      font.bold: lvItem.header
    }
  }

}

