import Qt 4.7

Item {
  id: bufferlistView

  property variant model

  ListView {
    anchors.fill: parent
    //contentY:  10000

    section.property: "section_item"
    section.criteria: ViewSection.FullString
    section.delegate: ListSectionItem {
      width: bufferlistView.width
      text: section // type=="Network" ? plaintext : "   "+plaintext;

//          MouseArea {
//            anchors.fill: parent
//            onClicked: {
//              parent.ListView.view.currentIndex = index
//            }
//          }
    }

    model: VisualDataModel {
      id: visualModel
      model: bufferlistView.model

      delegate: Column {
        id: networkItem
        width: parent.width

        ListItem {
          width: parent.width
          text: display // type=="Network" ? plaintext : "   "+plaintext;

//          MouseArea {
//            anchors.fill: parent
//            onClicked: {
//              parent.ListView.view.currentIndex = index
//            }
//          }
        }

        Column {
          width: parent.width

          Repeater {
            model: VisualDataModel {

              model: bufferlistView.model
              rootIndex: visualModel.modelIndex(index)

              delegate: ListItem {
                width: parent.width
                text: display // type=="Network" ? plaintext : "   "+plaintext;
                current: parent.ListView.isCurrentItem

      //          MouseArea {
      //            anchors.fill: parent
      //            onClicked: {
      //              // todo parent.ListView.view.currentIndex = index
      //            }
      //          }
              }
            }
          }

        }

      }
    }

    ScrollBar {
      scrollArea: parent
    }
  }
}
