import Qt 4.7
import org.quassel 0.1

Rectangle {
    id: background
    //anchors.fill:  parent

    width: 800
    height: 480

    color: "#000000"

    state:"default"

    Rectangle {
        id: modalblur
        color: "#000000"
        opacity: 0.0
        //visible: false
        z: 95

        property int shadowOrientation: 1;

        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top

        anchors.left: background.left
        anchors.right: background.right

        MouseArea {
            anchors.fill: parent
            onClicked: { background.state="default"; }
        }
    }

    Rectangle {
        id: chatview
        border.width: 1

        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top

        anchors.left: background.left
        anchors.right: background.right

        QuasselChatView {
            id: chatview_view
            anchors.left: parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom

            //scrollModel.verticalPosition: chatview_flickable.visibleArea.yPosition * chatview_flickable.contentHeight

            scrollModel.verticalPosition: chatview_flickable.contentY
            scrollModel.onVerticalPositionChanged: { if(!chatview_flickable.flicking && !chatview_flickable.moving) { chatview_flickable.contentY = scrollModel.verticalPosition } }

            // pos: (chatview_flickable.visibleArea.yPosition < 0.1 ? 0.1 : chatview_flickable.visibleArea.yPosition) * chatview_flickable.contentHeight
        }

        Flickable {
          id: chatview_flickable
          x: chatview_view.scrollModel.viewportPosition.x
          y: chatview_view.scrollModel.viewportPosition.y
          //anchors.left: parent.left
         // anchors.top: parent.top
          //anchors.bottom: parent.bottom
          height: chatview_view.scrollModel.viewportSize.height
          width: chatview_view.scrollModel.viewportSize.width
          contentHeight: height + (chatview_view.scrollModel.contentsSize.height <= 0 ? 0 : chatview_view.scrollModel.contentsSize.height)
          contentWidth: width
          // boundsBehavior: "DragOverBounds"

          MouseArea {
            id: chatview_tap
            property bool pressed: false
            anchors.fill: parent
            //onPressed: pressed = true
            //onReleased: pressed = false
            onClicked: {
              pressed = true
              pressed = false
            }
          }
        }

        ScrollBar {
          id: chatview_flickable_scroll
          scrollArea: chatview_flickable
        }

        Item {
          // TODO: make a component from this.

          id: chatview_flickable_scroll_handle
          height: 60
          width: 60
          opacity: 0.00
          anchors.left: chatview_flickable.left
          anchors.margins: 1

          y: chatview_flickable.contentY / (chatview_flickable.contentHeight-chatview_flickable.height) * (chatview_flickable.height-chatview_flickable_scroll_handle.height)

          Rectangle {
            anchors.fill: parent
            radius: 10
            border.width: 1
            border.color: "#ffffff"
            color: "#000000"
            opacity: 0.5
          }

          Image {
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: 22
            height: 22
            source: "image://quassel/keyboard_move_up"
          }
          Image {
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            width: 22
            height: 22
            source: "image://quassel/keyboard_move_down"
          }

          MouseArea {
              anchors {
                  fill: chatview_flickable_scroll_handle
                  verticalCenter: chatview_flickable_scroll_handle.verticalCenter
                  horizontalCenter: chatview_flickable_scroll_handle.horizontalCenter
              }

              drag {
                  target: chatview_flickable_scroll_handle
                  axis: Drag.YAxis
                  minimumY: 0
                  maximumY: chatview_flickable.height - chatview_flickable_scroll_handle.height
              }

              onPositionChanged: {
                chatview_flickable.contentY = chatview_flickable_scroll_handle.y * (chatview_flickable.contentHeight-chatview_flickable.height)  / (chatview_flickable.height-chatview_flickable_scroll_handle.height)
              }
              Component.onCompleted: {
                      chatview_flickable.contentY = Math.max(0, chatview_flickable_scroll_handle.y / chatview_flickable_scroll.height * chatview_flickable.contentHeight)
              }
          }

          states: State {
              name: "visible"
              when: chatview_flickable.moving
              PropertyChanges { target: chatview_flickable_scroll_handle; opacity: 1.0 }
          }

          transitions: Transition {
              from: "visible"; to: ""
              NumberAnimation { properties: "opacity"; duration: 600 }
          }
        }


        Item {
          id: statusWidget
          anchors.top: parent.top
          anchors.horizontalCenter: parent.horizontalCenter
          anchors.margins: 10
          width: parent.width * 0.8
          height: parent.height * 0.8

          Column {
            id: statusWidget_col
            anchors.fill: parent
            anchors.margins: 10
            spacing: 5

            Button {
              id: requestBacklogButton
              text: "Get Older Messages"
              visible: coreConnection.connected && ((chatview_flickable.height >= chatview_view.scrollModel.contentsSize.height) || (chatview_flickable.visibleArea.yPosition < 0.1))
              height: 60
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.margins: 10

              iconSource: "image://quassel/general_refresh"

              onClicked: {
                chatview_view.requestBacklog()
              }
            }

            ProgressWidget {
              id: connectionProgress
              text: coreConnection.progressText
              progressMin: coreConnection.progressMinimum
              progressMax: coreConnection.progressMaximum
              progressValue: coreConnection.progressValue
              visible: coreConnection.progressMaximum > 0
              width: parent.width
              height: 60
            }

            StatusMessageWidget {
              id: connectionProgress_Error
              width: parent.width
              height: 60

              text: ""
              visible: false

              Connections {
                target: coreConnection
                onConnectionError: {
                  connectionProgress_Error.text = errorMsg
                  connectionProgress_Error.visible = true
                }
              }
            }

            ProgressWidget {
              id: backlogProgress
              text: "Requesting Backlog ..." + backlogManagerStatus.progressMaximum
              progressMin: backlogManagerStatus.progressMinimum
              progressMax: backlogManagerStatus.progressMaximum
              progressValue: backlogManagerStatus.progressValue
              visible: backlogManagerStatus.progressActive
              width: parent.width
              height: 60
            }
          }
        }

        Item {
          id: menuWidget
          anchors.bottom: chatview_flickable.bottom
          anchors.right: fullscreenMenuWidget.left
          anchors.margins: 5
          width: menuWidget_row.width + 10
          height: menuWidget_row.height + 10
          opacity: 0.00

        Rectangle {
          anchors.fill: parent
          color: "#000000"
          opacity: 0.5
          radius: 10
          border.width: 1
          border.color: "#ffffff"
        }

        Row {
          id: menuWidget_row
          anchors.centerIn: parent
          spacing: 5

          ToolButton {
            id: searchButton
            // text: "+"
            width: 60
            height: 60
            onClicked: {
              ctxt.search()
            }

            icon: "image://quassel/general_search"
          }

          ToolButton {
            id: zoomInButton
            // text: "+"
            width: 60
            height: 60
            onClicked: {
              ctxt.zoomIn()
            }

            icon: "image://quassel/pdf_zoomin"
          }
          ToolButton {
            id: zoomOutButton
            // text: "-"
            width: 60
            height: 60
            onClicked: {
              ctxt.zoomOut()
            }

              icon: "image://quassel/pdf_zoomout"
          }
        }


        states: State {
            name: "visible"
            when: chatview_tap.pressed
            PropertyChanges { target: menuWidget; opacity: 1.0 }
        }

        transitions: Transition {
            from: "visible"; to: ""
            SequentialAnimation {
              PauseAnimation { duration: 1500 }
              NumberAnimation { properties: "opacity"; duration: 600 }
            }
        }
      }

        Item {
          id: columnWidget

          visible: (ctxt.firstColumn != undefined) //&& chatview_tap.tap_on
          opacity: menuWidget.opacity

          anchors.top: chatview_flickable.top
          anchors.bottom: chatview_flickable.bottom
          x: ctxt.firstColumn != undefined ? ctxt.firstColumn.columnPos : 0
          width: 60

          Rectangle {
            width: 3
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: columnWidget_rect.left
            anchors.margins: 3
            color: "#000000"
            border.width: 1
            border.color: "#ffffff"
            opacity: 0.8
          }

          Rectangle {
            id: columnWidget_rect
            height: 60
            width: 60
            radius: 10
            border.width: 1
            border.color: "#ffffff"
            color: "#000000"
            opacity: 0.5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            anchors.leftMargin: 4
          }

          MouseArea {
            anchors.fill: columnWidget_rect
            drag {
              target: parent
              axis: Drag.XAxis
            }
            onPositionChanged: {
              ctxt.firstColumn.columnPos = parent.x
            }
          }

          Image {
            anchors.centerIn: columnWidget_rect
            width: 32
            height: 32
            source: "image://quassel/browser_mover"
          }

        }

        Item {
          id: columnWidget2

          visible: (ctxt.secondColumn != undefined) //&& chatview_tap.tap_on
          opacity: menuWidget.opacity

          anchors.top: chatview_flickable.top
          anchors.bottom: chatview_flickable.bottom
          x: ctxt.secondColumn != undefined ? ctxt.secondColumn.columnPos : 0
          width: 60

          Rectangle {
            width: 3
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: columnWidget2_rect.left
            anchors.margins: 3
            color: "#000000"
            border.width: 1
            border.color: "#ffffff"
            opacity: 0.8
          }

          Rectangle {
            id: columnWidget2_rect
            height: 60
            width: 60
            radius: 10
            border.width: 1
            border.color: "#ffffff"
            color: "#000000"
            opacity: 0.5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.margins: 10
            anchors.leftMargin: 4
          }

          MouseArea {
            anchors.fill: columnWidget2_rect
            drag {
              target: parent
              axis: Drag.XAxis
            }
            onPositionChanged: {
              ctxt.secondColumn.columnPos = parent.x
            }
          }

          Image {
            anchors.centerIn: columnWidget2_rect
            width: 32
            height: 32
            source: "image://quassel/browser_mover"
          }

        }

        Item {
          id: fullscreenMenuWidget
          anchors.bottom: chatview_flickable.bottom
          anchors.right: chatview_flickable.right
          anchors.margins: 5
          anchors.rightMargin: 15
          width: fullScreenMenuWidget_row.width + 10
          height: fullScreenMenuWidget_row.height + 10
          // visible: chatview_tap.tap_on // chatview_flickable_scroll.visible
          opacity: 0.0

          Rectangle {
            anchors.fill: parent
            color: "#000000"
            opacity: 0.5
            radius: 10
            border.width: 1
            border.color: "#ffffff"
          }

          Row {
            id: fullScreenMenuWidget_row
            anchors.centerIn: parent
            spacing: 5

              ToolButton {
                id: fullScreenButton
                width: 60
                height: 60
                icon: "image://quassel/general_fullsize"
                onClicked: { ctxt.fullScreen = !ctxt.fullScreen; }
              }
          }

          states: State {
              name: "visible"
              when: chatview_tap.pressed || ctxt.fullScreen
              PropertyChanges { target: fullscreenMenuWidget; opacity: 1.0 }
          }

          transitions: Transition {
              from: "visible"; to: ""
              SequentialAnimation {
                PauseAnimation { duration: 1500 }
                NumberAnimation { properties: "opacity"; duration: 600 }
              }
          }
        }
    }

    Item {
        id: toolbar
        height: 100
        width: parent.width
        anchors.left: background.left
        anchors.right: background.right
        state: "default"

        z: 99

        Rectangle {
          anchors.fill: parent
          smooth: true

          gradient: Gradient {
            GradientStop {
              position: 0.0
              color: "#666666"
            }
            GradientStop {
              position:1.0
              color: "#111111"
            }
          }
        }

       Column {
          anchors.fill: parent

          ChatListView {
            id: chatListViewTop
            width: parent.width
            height: 60
            clip: true
            model: ctxt.allBuffersModel

            boundsBehavior: "StopAtBounds"

            orientation: "Horizontal"

            currentIndex: ctxt.currentBufferIndex

            section.property: ""

            delegate: Item {
              height: chatListViewTop.height
              width: 80

              Rectangle {
                anchors.fill: parent
                color: "#000000"
                visible: chatListViewTop.currentIndex != index
              }

              ListItem {
                anchors.fill: parent
                text:  display // type=="Network" ? plaintext : "   "+plaintext;
                textColor: "#ffffff" //
                iconSource: decorationIconName ? "image://quassel/" + decorationIconName : ""
                // current: chatListViewTop.currentIndex == index

                header: section_depth == 0

                Rectangle {
                  anchors.margins: 2
                  anchors.left: parent.left
                  anchors.right: parent.right
                  anchors.bottom: parent.bottom
                  height: 6
                  radius: 2

                  function isHighlight(act) {
                    var noActivity = 0;
                    var otherActivity = 1;
                    var newMessage = 2;
                    var highlight = 4;
                    return (act & (otherActivity + newMessage + highlight));
                  }

                  function fgColor(act) {
                    var noActivity = 0;
                    var otherActivity = 1;
                    var newMessage = 2;
                    var highlight = 4;
                    if(act & highlight)
                      return "#FF9647";
                    else if(act & newMessage)
                      return "#003BFF";
                    else if(act & otherActivity)
                      return "#008000";
                    else
                      return "#000000"
                  }

                  color: fgColor(bufferActivity)
                  visible: isHighlight(bufferActivity)
                }

                MouseArea {
                  anchors.fill: parent
                  onClicked: {
                    ctxt.setCurrentBufferIndex(index)
                  }
                }
              }
            }
          }

          Item {
              id: topicbar
              height: 40
              width: parent.width
              x: parent.x
              y: parent.y // parent.y - height

              Text {
                  id: topicText
                  text: topicModel.currentTopic
                  color: "#ffffff"
                  // anchors.verticalCenter: parent.verticalCenter
                  horizontalAlignment: "AlignLeft"
                  verticalAlignment: "AlignVCenter"
                  //height: parent.height
                  elide: Text.ElideRight
                  anchors.leftMargin: 10
                  anchors.rightMargin: 10
                  anchors.top:  parent.top
                  anchors.bottom: parent.bottom
                  anchors.left: parent.left
                  anchors.right: parent.right // nicksBtn.left

                  MouseArea {
                      anchors.fill: parent
                      onClicked: { background.state="TopicEditing"; }
                  }
              }
          }
        }

        transitions: [
            Transition {
                from: "default"
                SequentialAnimation {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                        targets: [topicbar, toolbar]
                        properties: "y,x"
                    }

                }
            },
            Transition {
                to: "default"
                SequentialAnimation {
                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                        targets:  [topicbar, toolbar]
                        properties: "y"
                    }
                }
            }
        ]

    }

    Rectangle {
        id: topicTextFull

        color: "#ffffff"
        ///border.width: 1
        radius: 5

        y: topicText.y
        anchors.fill: background
        anchors.margins: 60
        opacity: 0
        visible: false

        Column {
            id: topicTextFullTextLayout
            anchors.fill: parent
            anchors.margins: 10
            width: parent.width
            spacing: 10
            Text {
                id: topicTextFullText
                text: topicModel.currentTopic
                width: parent.width
                wrapMode: Text.WordWrap
            }
            Button {
                id: topicTextFullBtn
                visible: topicModel.readOnly
                text: "edit"
            }
        }

        MouseArea {
            anchors.fill: topicTextFull
            onClicked: { background.state="default"; }
        }
    }

    Item {
        id: inputbar
//        color: "#99ff99"
        height: quassel_input_widget.height > 60 ? quassel_input_widget.height : 60
        width: parent.width
        y: parent.y+parent.height-height
        // opacity: 0.6
        // border.width: 1

        z: 99

        QuasselNickWidget {
          id: quassel_nick_widget
          anchors.top: parent.top
          anchors.left: parent.left
          anchors.bottom: parent.bottom
          width: parent.width * 0.2
        }

        QuasselInputWidget {
          id: quassel_input_widget
          //anchors.top: parent.top
          y: 0 // parent.height > quassel_input_widget.heightHint ? 0 : (parent.height - quassel_input_widget.heightHint)
          height: 60 > quassel_input_widget.heightHint ? 60 : quassel_input_widget.heightHint
//          Behavior on y {
//            NumberAnimation { duration: 200 }
//          }
          Behavior on height {
            NumberAnimation { duration: 200 }
          }
          anchors.left: quassel_nick_widget.right
          anchors.right: parent.right
          //anchors.bottom: parent.bottom
        }
    }

    states: [
        State {
            name: "TopicEditing";
            PropertyChanges { target: toolbar; y:background.y-toolbar.height}
            PropertyChanges { target: inputbar; y:background.y+background.height}
            PropertyChanges { target: toolbar; state: "TopicEditing"}
            PropertyChanges { target: topicTextFull; y: background.y+10; visible: true; opacity:0.9;  z: 199}
            PropertyChanges { target: modalblur; anchors.top: background.top; opacity: 0.6; z: 198}
        }
    ]
    transitions: [
        Transition { NumberAnimation { properties: "x,y,opacity"; duration: 150; easing.type: Easing.InOutQuad } }
    ]
}
