import Qt 4.7
import org.quassel 0.1

Rectangle {
    id: background
    //anchors.fill:  parent

    width: 800
    height: 480

    color: "#000000"

    property bool chatListDocked: false;
    property bool nickListDocked: false;

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

        anchors.left: chatListDocked?bufferlist.right:background.left
        anchors.right: nickListDocked?nicklist.left:background.right

        QuasselChatView {
            id: chatview_view
            anchors.left: parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom

            //scrollModel.verticalPosition: chatview_flickable.visibleArea.yPosition * chatview_flickable.contentHeight

            scrollModel.verticalPosition: chatview_flickable.contentY
            scrollModel.onVerticalPositionChanged: { if(!chatview_flickable.flicking) { chatview_flickable.contentY = scrollModel.verticalPosition } }

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
        }

        ScrollBar {
          scrollArea: chatview_flickable
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
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.margins: 10

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
    }

    Item {
        id: toolbar
        height: 110
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
            width: parent.width
            height: 50
            clip: true
            model: ctxt.allBuffersModel

            orientation: "Horizontal"

            currentIndex: ctxt.currentBufferIndex

            section.property: ""

            delegate: Item {
              height: parent.height
              width: 80

              Rectangle {
                anchors.fill: parent
                color: "#000000"
                visible: listViewItem.currentIndex != index
              }

              ListItem {
                anchors.fill: parent
                text: display // type=="Network" ? plaintext : "   "+plaintext;
                textColor: "#ffffff"
                iconSource: "image://quassel/" + decorationIconName
                // current: listViewItem.currentIndex == index

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
              height: 60
              width: parent.width
              x: parent.x
              y: parent.y // parent.y - height

//              ToolButton {
//                  id: chatsBtn
//                  // text: "Chats"
//                  icon: "image://quassel/general_foldertree"
//                  height: parent.height
//                  anchors.left: parent.left
//                  anchors.top: parent.top
//                  onClicked: background.state="showChatList"
//              }

              ToolButton {
                  id: nicksBtn
                  // text: "Nicks"
                  icon: "image://quassel/general_contacts_button" // general_conference_avatar
                  height: parent.height
                  anchors.right: parent.right
                  anchors.top: parent.top
                  onClicked: background.state="showNickList"
              }

              Text {
                  id: topicText
                  text: topicModel.currentTopic
                  color: "#ffffff"
                  anchors.verticalCenter: parent.verticalCenter
                  //height: parent.height
                  elide: Text.ElideRight
                  anchors.leftMargin: 10
                  anchors.rightMargin: 10
                  anchors.left: chatsBtn.right
                  anchors.right: nicksBtn.left

                  MouseArea {
                      anchors.fill: parent
                      onClicked: { background.state="TopicEditing"; }
                  }
              }
          }
        }

//        states: [
//            State {
//                name: "showChatList";
//                PropertyChanges { target: topicbar; y:parent.y-parent.height}
//                PropertyChanges { target: chatlistbar; y:parent.y}
//            },
//            State {
//                name: "showNickList";
//                PropertyChanges { target: topicbar; y:parent.y-parent.height}
//                PropertyChanges { target: nicklistbar; y:parent.y}
//            }
//        ]


//        transitions: [
//            Transition { NumberAnimation { properties: "x,y,opacity"; duration: 150; easing.type: Easing.InOutQuad } }
//        ]

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

                    NumberAnimation {
                        duration: 150
                        easing.type: Easing.InOutQuad
                        targets: [nicklistbar, chatlistbar]
                        properties: "y,x"
                    }
                }
            },
            Transition {
                to: "default"
                NumberAnimation {
                    duration: 150
                    easing.type: Easing.InOutQuad
                    targets: [nicklistbar, chatlistbar]
                    properties: "y"
                }
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
        height: 60
        width: parent.width
        y: parent.y+parent.height-height
        // opacity: 0.6
        // border.width: 1

        z: 99

        QuasselInputWidget {
          id: quassel_input_widget
          anchors.top: parent.top
          height: parent.height
          anchors.left: parent.left
          anchors.right: rightInputBtnRow.left
        }

        Row {
          id: rightInputBtnRow
          anchors.top: parent.top
          anchors.right: parent.right
          height: parent.height
          // TODO: actions for zoomin/zoomout
          ToolButton {
            icon: "image://quassel/general_fullsize"
            onClicked: { ctxt.fullScreen = !ctxt.fullScreen; }
          }
        }

//        Button {
//            id: nickSelectBtn
//            text: "nick"
//            height: parent.height
//            anchors.left: parent.left
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//        }
//        Input {
//            text: "text input"
//            anchors.right: parent.right
//            anchors.top: parent.top
//            anchors.bottom: parent.bottom
//            anchors.left: nickSelectBtn.right
//            focus:true
//        }
    }



    Rectangle {
        id: bufferlist
        width: 300
        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top
        x: chatListDocked ? background.x : background.x-width
        border.width: 1

        ChatListView {
            anchors.fill: parent
            model: ctxt.allBuffersModel

            currentIndex: ctxt.currentBufferIndex

//            MouseArea {
//                anchors.fill: parent
//                property int dragStartSize: 0;
//                property int dragStartX: 0;

//                onPressed: {
//                  console.log("pressed...");
//                    dragStartSize = bufferlist.width
//                    dragStartX = mapToItem(background,mouse.x,0).x
//                }

//                onReleased: {
//                    dragStartX = -1
//                }

//                onMousePositionChanged: {
//                        bufferlist.width = dragStartSize + (mapToItem(background,mouse.x,0).x - dragStartX)
//                }
//            }
        }
    }

    Rectangle {
        id: bufferlistGrip
        width: 60
        anchors.left: bufferlist.right
        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top
        border.width: 1
        opacity: 0
        visible: false

        ToolButton {
            id: bufferlistGripDockBtn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: toolbar.height
            icon: "image://quassel/general_locked"
            // text: "Dock"
            checked: background.chatListDocked
            onClicked: {
                background.chatListDocked = !background.chatListDocked;
                if(background.chatListDocked){background.state="default";}
            }
        }
        Item {
            anchors.left: parent.left
            anchors.top: bufferlistGripDockBtn.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            Text {
                anchors.centerIn: parent
                text: "<>"
            }
            MouseArea {
                anchors.fill: parent
                property int dragStartSize: 0;
                property int dragStartX: 0;

                onPressed: {
                    dragStartSize = bufferlist.width
                    dragStartX = mapToItem(background,mouse.x,0).x
                }

                onReleased: {
                    dragStartX = -1
                }

                onMousePositionChanged: {
                        bufferlist.width = dragStartSize + (mapToItem(background,mouse.x,0).x - dragStartX)
                }
            }
        }
    }

    Rectangle {
        id: nicklistGrip
        width: 60
        anchors.right: nicklist.left
        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top
        border.width: 1
        opacity: 0
        visible: false

        ToolButton {
            id: nickGripDockBtn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: toolbar.height
            // text: "Dock"
            icon: "image://quassel/general_locked"
            checked: background.nickListDocked
            onClicked: {
                background.nickListDocked = !background.nickListDocked;
                if(background.nickListDocked){background.state="default";}
            }
        }
        Item {
            anchors.left: parent.left
            anchors.top: nickGripDockBtn.bottom
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            Text {
                anchors.centerIn: parent
                text: "<>"
            }
            MouseArea {
                anchors.fill: parent
                property int dragStartSize: 0;
                property int dragStartX: 0;

                onPressed: {
                    dragStartSize = nicklist.width
                    dragStartX = mapToItem(background,mouse.x,0).x
                }

                onReleased: {
                    dragStartX = -1
                }

                onMousePositionChanged: {
                        nicklist.width = dragStartSize - (mapToItem(background,mouse.x,0).x - dragStartX)
                }
            }
        }
    }

    Rectangle {
        id: nicklist
        width: 300
        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top
        x: nickListDocked ? background.x+background.width-width : background.x+background.width
        border.width: 1

        NickListView {
            anchors.fill: parent
            model:  ctxt.channelUsersModel
        }
    }

    states: [
        State {
            name: "showChatList";
            PropertyChanges { target: toolbar; y:background.y-toolbar.height}
            PropertyChanges { target: inputbar; y:background.y+background.height}
            PropertyChanges { target: bufferlist; x: 0; opacity: 0.9; z: 96}
            PropertyChanges { target: bufferlistGrip; visible: true; opacity: 0.9; z: 96}
            PropertyChanges { target: modalblur; anchors.left:bufferlistGrip.right; opacity: 0.6}
        },
        State {
            name: "showNickList";
            PropertyChanges { target: toolbar; y:background.y-toolbar.height}
            PropertyChanges { target: inputbar; y:background.y+background.height}
            PropertyChanges { target: nicklist; x: background.x+background.width-nicklist.width; opacity: 0.9; z: 96}
            PropertyChanges { target: nicklistGrip; visible: true; opacity: 0.9; z: 96}
            PropertyChanges { target: modalblur; anchors.right:nicklistGrip.left; opacity: 0.6}
        },
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
