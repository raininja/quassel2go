import Qt 4.7

Item {
  id: progress
  property string text: ""
  property real progress: (progressMax-progressMin) <= 0 ? 0 : (progressValue-progressMin)/(progressMax-progressMin)
  property int progressMin: 0
  property int progressMax: 100
  property int progressValue: 50
  width: 300
  height: max(text.height, bar.height)

  Rectangle {
    anchors.fill: parent
    color: "white"
    radius: 10
    opacity: 0.6
  }

  Row {
    anchors.fill:  parent
    anchors.margins: 10
    spacing: 10

    Rectangle {
      id: bar
      width: parent.width * 0.2
      height: 20
      anchors.verticalCenter: parent.verticalCenter
      radius: 5
      border.width: 2
      border.color: "#000000"

      Rectangle {
        anchors.margins: 3
        radius: 2
        anchors.left: parent.left
        anchors.top:  parent.top
        anchors.bottom:  parent.bottom
        width: (parent.width - 2*anchors.margins) * progress.progress
        color: "#000000"
      }
    }

    Text {
      id: text
      width: parent.width * 0.8
      wrapMode: "WordWrap"
      text: progress.text
      anchors.verticalCenter: parent.verticalCenter
    }

  }
}
