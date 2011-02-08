import Qt 4.7

ListView {
  id: listView
  anchors.fill: parent

  delegate: ListItem {
    width: listView.width
    text: display // type=="Network" ? plaintext : "   "+plaintext;
    iconSource: decorationIconName ? "image://quassel/" + decorationIconName : ""
    current: ListView.view.currentIndex == index

    header: section_depth == 0

    MouseArea {
      anchors.fill: parent
      onClicked: {
        ListView.view.currentIndex = index
      }
    }
  }

  ScrollBar {
    scrollArea: parent
  }
}

