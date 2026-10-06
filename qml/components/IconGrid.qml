import QtQuick
import QtQuick.Controls

GridView {
    id: root

    property alias model: root.model

    cellWidth: 120
    cellHeight: 130

    clip: true

    implicitHeight: contentHeight

    delegate: IconCard {
        width: root.cellWidth - 8
        height: root.cellHeight - 8

        iconName: model.name
        iconSource: model.source
    }
}