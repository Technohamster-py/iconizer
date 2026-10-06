#pragma once

#include <QObject>

class FolderIconInstaller : public QObject
{
    Q_OBJECT

public:
    explicit FolderIconInstaller(QObject *parent = nullptr);

    Q_INVOKABLE bool installIcon(const QString &folderPath,
                                 const QString &iconPath);

    Q_INVOKABLE bool removeIcon(const QString &folderPath);

    Q_INVOKABLE bool hasCustomIcon(const QString &folderPath) const;

    signals:
        void installationError(const QString &error);
};