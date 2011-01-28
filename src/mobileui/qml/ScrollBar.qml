import Qt 4.7

Item {
    id: scrollbar

    property variant scrollArea
    property variant orientation: Qt.Vertical

    anchors.right: scrollArea.right
    y: scrollArea.visibleArea.yPosition * scrollArea.height
    width: 10
    height: scrollArea.visibleArea.heightRatio * scrollArea.height
    opacity: 0

    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        color: "black"
        radius: 3
    }

    states: State {
        name: "visible"
        when: scrollArea.orientation == Qt.Vertical ? scrollArea.movingVertically : scrollArea.movingHorizontally
        PropertyChanges { target: scrollbar; opacity: 1.0 }
    }

    transitions: Transition {
        from: "visible"; to: ""
        NumberAnimation { properties: "opacity"; duration: 600 }
    }
}
