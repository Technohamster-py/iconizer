import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ColumnLayout {
    id: root

    spacing: 24

    // Модель передается из Main.qml.
    required property var iconModel

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
        model: root.iconModel
        builtinOnly: true
    }

    Label {
        text: qsTr("Мои иконки")
        font.pixelSize: 18
        font.bold: true

        visible: root.iconModel.count > 0
    }

    IconGrid {
        Layout.fillWidth: true
        model: root.iconModel
        builtinOnly: false
    }
}