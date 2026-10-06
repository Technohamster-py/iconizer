import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    spacing: 24

    Label {
        text: qsTr("Иконки")
        font.pixelSize: 20
        font.bold: true
    }

    Label {
        text: qsTr("Встроенные")
        font.pixelSize: 18
        font.bold: true
    }

    IconGrid {
        Layout.fillWidth: true
        model: builtinIcons
    }

    Label {
        text: qsTr("Мои иконки")
        font.pixelSize: 18
        font.bold: true

        visible: userIcons.count > 0
    }

    IconGrid {
        Layout.fillWidth: true
        model: userIcons

        visible: userIcons.count > 0
    }
}