import Qt 4.7

Item {
    id: scrollbar

    property variant scrollArea
    property variant orientation: Qt.Vertical
    property bool showAlways: false

    anchors.right: scrollArea.right
    anchors.margins: 1
    y: scrollArea.visibleArea.yPosition * scrollArea.height
    width: 10
    height: scrollArea.visibleArea.heightRatio * scrollArea.height
    opacity: 0.0

    Rectangle {
        anchors.fill: parent
        color: "black"
        radius: height < 4 ? 0 : 2
        border.width: 1
        border.color: "white"
        opacity: 0.85
    }

    states: State {
        name: "visible"
        when: scrollbar.showAlways || scrollArea.moving || (scrollArea.orientation == Qt.Vertical ? scrollArea.movingVertically : scrollArea.movingHorizontally)
        PropertyChanges { target: scrollbar; opacity: 1.0 }
    }

    transitions: Transition {
        from: "visible"; to: ""
        NumberAnimation { properties: "opacity"; duration: 600 }
    }
}
