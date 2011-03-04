import Qt 4.7

Item {
    id: scrollbar

    property variant scrollArea
    property variant orientation: Qt.Vertical
    property bool showAlways: false

    //anchors.right: scrollArea.right
    // anchors.margins: 1
    x: scrollbar.orientation == Qt.Vertical ? (scrollArea.width - width -2) : scrollArea.visibleArea.xPosition * scrollArea.width
    y: scrollbar.orientation == Qt.Vertical ? scrollArea.visibleArea.yPosition * scrollArea.height : (scrollArea.height - height +2)
    width: scrollbar.orientation == Qt.Vertical ? 10 : scrollArea.visibleArea.widthRatio * scrollArea.width
    height: scrollbar.orientation == Qt.Vertical ? scrollArea.visibleArea.heightRatio * scrollArea.height : 10
    opacity: 0.0

    Rectangle {
        anchors.fill: parent
        color: "black"
        radius: (height < 4 || width < 4) ? 0 : 2
        border.width: 1
        border.color: "white"
        opacity: 0.5
    }

    states: State {
        name: "visible"
        when: scrollbar.showAlways || scrollArea.moving || (scrollbar.orientation == Qt.Vertical ? scrollArea.movingVertically : scrollArea.movingHorizontally)
        PropertyChanges { target: scrollbar; opacity: 1.0 }
    }

    transitions: Transition {
        from: "visible"; to: ""
        NumberAnimation { properties: "opacity"; duration: 600 }
    }
}
