#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "models/iconmodel.h"
#include "models/iconfiltermodel.h"

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    IconModel iconModel;

    IconFilterModel builtinIcons;
    builtinIcons.setSourceModel(&iconModel);
    builtinIcons.setBuiltin(true);

    IconFilterModel userIcons;
    userIcons.setSourceModel(&iconModel);
    userIcons.setBuiltin(false);

    engine.rootContext()->setContextProperty(QStringLiteral("builtinIcons"), &builtinIcons);
    engine.rootContext()->setContextProperty(QStringLiteral("userIcons"), &userIcons);

    engine.loadFromModule("Iconizer", "Main");

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}