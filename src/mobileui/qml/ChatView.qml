import Qt 4.7

ListView {
    id: flickable

    delegate: Text { text: plaintext; wrapMode: Text.Wrap; width: parent.width }
    contentY:  10000

    ScrollBar {
        scrollArea: parent
    }
}
