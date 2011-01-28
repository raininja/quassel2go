import Qt 4.7

Rectangle {
    id: background
    width:  ctxt?ctxt.width:800
    height: ctxt?ctxt.height:480
    gradient: Gradient {
        GradientStop {
            position: 0.00;
            color: "#ffffff";
        }
        GradientStop {
            position: 0.35;
            color: "#ffffff";
        }
        GradientStop {
            position: 1.00;
            color: "#ffffff";
        }
    }

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
//        color: "#9999ff"
//        opacity: 0.6
        border.width: 1

        anchors.top: toolbar.bottom
        anchors.bottom: inputbar.top

        anchors.left: chatListDocked?bufferlist.right:background.left
        anchors.right: nickListDocked?nicklist.left:background.right

        //x: chatListDocked?bufferlist.width:0
        //width: background.width - (chatListDocked?bufferlist.width:0) - (nickListDocked?nicklist.width:0)

//        Text{
//            text: "chatview"
//            anchors.centerIn: parent
//        }

        ChatView {
            id: chatview_view
            anchors.left: parent.left
            anchors.right:parent.right
            anchors.top: parent.top
            anchors.bottom:parent.bottom

            model: ChatModel { }
        }
    }

    Item {
        id: toolbar
        height: 60
        width: parent.width
        anchors.left: background.left
        anchors.right: background.right
        state: "default"

        z: 99

        Rectangle {
            id: topicbar
            height: parent.height
            width: parent.width
            x: parent.x
            y: parent.y // parent.y - height

            Button {
                id: chatsBtn
                text: "Chats"
                height: parent.height
                anchors.left: parent.left
                anchors.top: parent.top
                onClicked: background.state="showChatList"
            }

            Button {
                id: nicksBtn
                text: "Nicks"
                height: parent.height
                anchors.right: parent.right
                anchors.top: parent.top
                onClicked: background.state="showNickList"
            }

            Text {
                id: topicText
                text: "Latest Qt: 4.7.1 | docs: qt.nokia.com/doc/ | #qt-creator | Bugs: bugreports.qt.nokia.com | Off-topic: #qt-chat | Forum: www.qtcentre.org | Mailing list:  lists.trolltech.com/qt-interest | Nokia devices and NDK: www.forum.nokia.com | Symbian: #qt-symbian | Maemo: #qt-maemo"
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
                text: "Latest Qt: 4.7.1 | docs: qt.nokia.com/doc/ | #qt-creator | Bugs: bugreports.qt.nokia.com | Off-topic: #qt-chat | Forum: www.qtcentre.org | Mailing list:  lists.trolltech.com/qt-interest | Nokia devices and NDK: www.forum.nokia.com | Symbian: #qt-symbian | Maemo: #qt-maemo"
                width: parent.width
                wrapMode: Text.WordWrap
            }
            Button {
                id: topicTextFullBtn
                text: "edit"
            }
        }

        MouseArea {
            anchors.fill: topicTextFull
            onClicked: { background.state="default"; }
        }
    }

    Rectangle {
        id: inputbar
//        color: "#99ff99"
        height: 60
        width: parent.width
        y: parent.y+parent.height-height
//        opacity: 0.6
        border.width: 1

        z: 99

        Button {
            id: nickSelectBtn
            text: "nick"
            height: parent.height
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }
        Input {
            text: "text input"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.left: nickSelectBtn.right
            focus:true
        }
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
            model: ChatListModel{}
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

        Button {
            id: bufferlistGripDockBtn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: toolbar.height
            text: "Dock"
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

        Button {
            id: nickGripDockBtn
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            height: toolbar.height
            text: "Dock"
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
