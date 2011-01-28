import Qt 4.7

ListModel {
    id: chatModel

    ListElement { plaintext: "BitlBee"; type: "Network" }
    ListElement { plaintext: "&bitlbee"; type: "Chat" }
    ListElement { plaintext: "tom"; type: "Query" }
    ListElement { plaintext: "peter"; type: "Query" }
    ListElement { plaintext: "anna"; type: "Query" }
    ListElement { plaintext: "freenode"; type: "Network" }
    ListElement { plaintext: "#emacs"; type: "Chat" }
    ListElement { plaintext: "#qt"; type: "Chat" }
    ListElement { plaintext: "#qt-maemo"; type: "Chat" }
    ListElement { plaintext: "#quassel"; type: "Chat" }
    ListElement { plaintext: "#maemo"; type: "Chat" }
}
