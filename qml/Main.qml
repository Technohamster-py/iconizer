import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    width: 900
    height: 650

    visible: true
    title: qsTr("Iconizer")

    IconLibrary {
        anchors.fill: parent
        anchors.margins: 20

        iconModel: icons
    }
}