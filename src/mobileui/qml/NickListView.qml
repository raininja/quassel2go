import Qt 4.7

Item {
  id: listView

  property variant model

  ListView {
    anchors.fill: parent
    //contentY:  10000

    section.property: "section_item"
    section.criteria: ViewSection.FullString
    section.delegate: ListSectionItem {
      width: listView.width
      text: section // type=="Network" ? plaintext : "   "+plaintext;
    }

    model: listView.model

    delegate: ListItem {
      width: parent.width
      text: display // type=="Network" ? plaintext : "   "+plaintext;
      current: ListView.view.currentIndex == index

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
}

