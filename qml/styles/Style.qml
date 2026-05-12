pragma Singleton
import QtQuick

QtObject{
    property color primaryColor: "#2196F3"
    property color primaryDarkColor: "#1976D2"
    property color primaryLightColor: "#BBDEFB"
    property color accentColor: "#FF4081"
    property color backgroundColor: "#F5F5F5"
    property color surfaceColor: "#FFFFFF"
    property color errorColor: "#F44336"
    property color successColor: "#4CAF50"
    property color textColor: "#212121"
    property color secondaryTextColor: "#757575"

    // Размеры
    property int spacing: 16
    property int largeSpacing: 24
    property int buttonHeight: 48
    property int iconSize: 64

    // Текст
    property string fontFamily: "Roboto, Arial, sans-serif"
    property int normalFontSize: 14
    property int titleFontSize: 20

    // Скругление углов
    property int borderRadius: 8

    // Тени
    property int elevation: 2
}