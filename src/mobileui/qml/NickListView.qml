import Qt 4.7

//Item {
//  id: listView

//  property variant model

  ListView {
    id: listView
    anchors.fill: parent
    //contentY:  10000

    section.property: "section_item"
    section.criteria: ViewSection.FullString
    section.delegate: ListSectionItem {
      width: parent.width
      text: section // type=="Network" ? plaintext : "   "+plaintext;
    }

//    model: listView.model

    delegate: ListItem {
      width: parent.width
      text: display // type=="Network" ? plaintext : "   "+plaintext;
      iconSource: "image://quassel/" + decorationIconName
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
//}

