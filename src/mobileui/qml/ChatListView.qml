import Qt 4.7

ListView {
  id: listView

  //    section.property: "section_item"
  //    section.criteria: ViewSection.FullString
  //    section.delegate: ListSectionItem {
  //      width: parent.width
  //      text: section // type=="Network" ? plaintext : "   "+plaintext;
  //    }

  delegate: ListItem {
    width: listView.width
    text: display // type=="Network" ? plaintext : "   "+plaintext;
    iconSource: decorationIconName ? "image://quassel/" + decorationIconName : ""
    current: listView.currentIndex == index

    header: section_depth == 0

    MouseArea {
      anchors.fill: parent
      onClicked: {
        console.log("about to change current from " + listView._cur)
        ctxt.setCurrentBufferIndex(index)
      }
    }
  }

  ScrollBar {
    scrollArea: parent
  }
}
