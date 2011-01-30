import Qt 4.7

Item {
  id: container

  property string icon: ""
  property bool checked: false
  signal clicked

  width: 60
  height: 60

  Rectangle {
      id: rect
      anchors.fill: parent
      radius: 5
      color: container.state == "Pressed" ? "#8888FF" : "#888888"
      opacity: container.state == "Pressed" ? 1 : 0
  }

  Image {
    anchors.centerIn: parent
    width: 48
    height: 48
    source: parent.icon
  }
  MouseArea {
    id: mouseRegion
      anchors.fill: parent
      onClicked: { parent.clicked(); }
  }

  states: [
      State {
          name: "Pressed"
          when: mouseRegion.pressed == true || container.checked == true
          // PropertyChanges { target: pressed; opacity: 1 }
      },
      State {
          name: "Focused"
          when: container.activeFocus == true
          PropertyChanges { target: btnText; color: "#FFFFFF" }
      }
  ]
}
