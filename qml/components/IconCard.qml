import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Frame {
    id: root

    property string iconName
    property url iconSource

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 6

        Image {
            Layout.fillWidth: true
            Layout.fillHeight: true

            source: root.iconSource
            fillMode: Image.PreserveAspectFit

            asynchronous: true
        }

        Label {
            Layout.fillWidth: true

            text: root.iconName
            horizontalAlignment: Text.AlignHCenter
            elide: Text.ElideRight
        }
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            // Позже здесь будет выбор иконки.
        }
    }
}