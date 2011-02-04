import Qt 4.7

//Item {

//  property variant model

  ListView {
    // property int currentIndex: -1

    //currentIndex:

    id: listViewItem
    //anchors.fill: parent
    //contentY:  10000

    section.property: "section_item"
    section.criteria: ViewSection.FullString
    section.delegate: ListSectionItem {
      width: parent.width
      text: section // type=="Network" ? plaintext : "   "+plaintext;
    }

    //model: parent.model

    delegate: ListItem {
      width: listViewItem.width
      text: display // type=="Network" ? plaintext : "   "+plaintext;
      iconSource: "image://quassel/" + decorationIconName
      current: listViewItem.currentIndex == index

      MouseArea {
        anchors.fill: parent
        onClicked: {
          console.log("about to change current from " + listViewItem._cur)
          //listViewItem.currentIndex = index
          ctxt.setCurrentBufferIndex(index)
        }
      }
    }

    ScrollBar {
      scrollArea: parent
    }
  }
//}
