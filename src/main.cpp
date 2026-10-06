#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "models/iconmodel.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    IconModel iconModel;

    engine.rootContext()->setContextProperty(QStringLiteral("icons"), &iconModel);
    engine.loadFromModule("Iconizer", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}