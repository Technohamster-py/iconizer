#include "foldericoninstaller.h"

#include <QBuffer>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QImageWriter>
#include <QPainter>
#include <QRegularExpression>
#include <QSvgRenderer>

#ifdef Q_OS_WIN
#include <windows.h>
#include <shlobj.h>
#endif

namespace {

constexpr auto IconFileName = "Iconizer.ico";
constexpr auto DesktopIniName = "desktop.ini";
constexpr auto BackupFileName = "desktop.ini.iconizer-backup";

struct IcoEntry
{
    quint8 width;
    quint8 height;
    quint8 colorCount;
    quint8 reserved;
    quint16 planes;
    quint16 bitCount;
    quint32 size;
    quint32 offset;
};

} // namespace

FolderIconInstaller::FolderIconInstaller(QObject* parent)
    : QObject(parent)
{
}

bool FolderIconInstaller::installIcon(const QString& folderPath,
                                      const QString& iconPath)
{
#ifndef Q_OS_WIN
    Q_UNUSED(folderPath)
    Q_UNUSED(iconPath)

    return fail(QStringLiteral("Установка иконок поддерживается только в Windows."));
#else
    const QDir folder(folderPath);

    if (!folder.exists())
        return fail(QStringLiteral("Целевая папка не существует: %1").arg(folderPath));

    if (iconPath.isEmpty())
        return fail(QStringLiteral("Не указан файл иконки."));

    const QFileInfo sourceInfo(iconPath);

    if (!sourceInfo.exists() || !sourceInfo.isFile())
        return fail(QStringLiteral("Файл иконки не существует: %1").arg(iconPath));

    const QString desktopIniPath = folder.filePath(DesktopIniName);
    const QString backupPath = folder.filePath(BackupFileName);
    const QString iconFilePath = folder.filePath(IconFileName);

    // Если desktop.ini ещё не принадлежит Iconizer, сохраняем его перед заменой.
    if (QFile::exists(desktopIniPath) && !hasCustomIcon(folderPath)) {
        QFile::remove(backupPath);

        if (!QFile::copy(desktopIniPath, backupPath))
            return fail(QStringLiteral("Не удалось создать резервную копию desktop.ini."));
    }

    if (!createIco(iconPath, iconFilePath)) {
        if (QFile::exists(backupPath) && !QFile::exists(desktopIniPath))
            QFile::copy(backupPath, desktopIniPath);

        return false;
    }

    if (!writeDesktopIni(folderPath))
        return false;

    if (!setWindowsAttributes(folderPath))
        return false;

    notifyShell(folderPath);

    return true;
#endif
}

bool FolderIconInstaller::removeIcon(const QString& folderPath)
{
#ifndef Q_OS_WIN
    Q_UNUSED(folderPath)

    return fail(QStringLiteral("Удаление иконок поддерживается только в Windows."));
#else
    const QDir folder(folderPath);

    if (!folder.exists())
        return fail(QStringLiteral("Целевая папка не существует: %1").arg(folderPath));

    if (!hasCustomIcon(folderPath))
        return true;

    const QString desktopIniPath = folder.filePath(DesktopIniName);
    const QString backupPath = folder.filePath(BackupFileName);
    const QString iconPath = folder.filePath(IconFileName);

    SetFileAttributesW(reinterpret_cast<LPCWSTR>(iconPath.utf16()), FILE_ATTRIBUTE_NORMAL);
    QFile::remove(iconPath);

    SetFileAttributesW(reinterpret_cast<LPCWSTR>(desktopIniPath.utf16()), FILE_ATTRIBUTE_NORMAL);
    QFile::remove(desktopIniPath);

    // Восстанавливаем desktop.ini, который существовал до Iconizer.
    if (QFile::exists(backupPath)) {
        if (!QFile::rename(backupPath, desktopIniPath))
            return fail(QStringLiteral("Не удалось восстановить исходный desktop.ini."));

        setWindowsAttributes(folderPath);
    } else {
        QFile::remove(backupPath);

        SetFileAttributesW(reinterpret_cast<LPCWSTR>(folderPath.utf16()),
                           FILE_ATTRIBUTE_NORMAL);
    }

    notifyShell(folderPath);

    return true;
#endif
}

bool FolderIconInstaller::hasCustomIcon(const QString& folderPath) const
{
#ifndef Q_OS_WIN
    Q_UNUSED(folderPath)

    return false;
#else
    const QString desktopIniPath = QDir(folderPath).filePath(DesktopIniName);

    QFile file(desktopIniPath);

    if (!file.open(QIODevice::ReadOnly))
        return false;

    const QString contents = QString::fromUtf8(file.readAll());

    // Проверяем не просто наличие desktop.ini, а принадлежность Iconizer.
    return contents.contains(QStringLiteral("IconFile=Iconizer.ico"),
                             Qt::CaseInsensitive);
#endif
}

bool FolderIconInstaller::createIco(const QString& sourcePath,
                                     const QString& destinationPath)
{
    const QFileInfo sourceInfo(sourcePath);

    if (sourceInfo.suffix().compare(QStringLiteral("ico"), Qt::CaseInsensitive) == 0) {
        QFile::remove(destinationPath);

        if (!QFile::copy(sourcePath, destinationPath))
            return fail(QStringLiteral("Не удалось скопировать ICO-файл: %1").arg(sourcePath));

        return true;
    }

    const QImage image = loadImage(sourcePath);

    if (image.isNull())
        return fail(QStringLiteral("Не удалось загрузить изображение: %1").arg(sourcePath));

    if (!writeIco(image, destinationPath))
        return fail(QStringLiteral("Не удалось создать ICO-файл: %1").arg(destinationPath));

    return true;
}

QImage FolderIconInstaller::loadImage(const QString& sourcePath) const
{
    const QFileInfo info(sourcePath);

    if (info.suffix().compare(QStringLiteral("svg"), Qt::CaseInsensitive) == 0) {
        QSvgRenderer renderer(sourcePath);

        if (!renderer.isValid())
            return {};

        const QSize defaultSize = renderer.defaultSize().isValid()
                                       ? renderer.defaultSize()
                                       : QSize(256, 256);

        QImage image(defaultSize, QImage::Format_ARGB32);
        image.fill(Qt::transparent);

        QPainter painter(&image);
        renderer.render(&painter);

        return image;
    }

    return QImage(sourcePath);
}

bool FolderIconInstaller::writeIco(const QImage& sourceImage,
                                    const QString& destinationPath) const
{
    const QList<int> sizes = {256, 128, 64, 48, 32, 24, 16};

    struct IcoImage
    {
        QByteArray data;
        int size;
    };

    QVector<IcoImage> images;
    images.reserve(sizes.size());

    for (const int size : sizes) {
        const QImage image = sourceImage.scaled(size,
                                                size,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation);

        QByteArray pngData;
        QBuffer buffer(&pngData);
        buffer.open(QIODevice::WriteOnly);

        QImageWriter writer(&buffer, "png");

        if (!writer.write(image))
            return false;

        images.append({pngData, size});
    }

    QFile file(destinationPath);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    auto write16 = [&file](quint16 value) {
        const char bytes[] = {
            static_cast<char>(value & 0xff),
            static_cast<char>((value >> 8) & 0xff)
        };

        return file.write(bytes, sizeof(bytes)) == sizeof(bytes);
    };

    auto write32 = [&file](quint32 value) {
        const char bytes[] = {
            static_cast<char>(value & 0xff),
            static_cast<char>((value >> 8) & 0xff),
            static_cast<char>((value >> 16) & 0xff),
            static_cast<char>((value >> 24) & 0xff)
        };

        return file.write(bytes, sizeof(bytes)) == sizeof(bytes);
    };

    // ICONDIR.
    if (!write16(0) || !write16(1) || !write16(images.size()))
        return false;

    const quint32 directorySize = 6 + images.size() * 16;
    quint32 offset = directorySize;

    QVector<IcoEntry> entries;
    entries.reserve(images.size());

    for (const IcoImage& image : images) {
        const quint8 dimension = image.size >= 256
                                     ? 0
                                     : static_cast<quint8>(image.size);

        entries.append({
            dimension,
            dimension,
            0,
            0,
            1,
            32,
            static_cast<quint32>(image.data.size()),
            offset
        });

        offset += image.data.size();
    }

    for (const IcoEntry& entry : entries) {
        if (file.write(reinterpret_cast<const char*>(&entry.width), 1) != 1 ||
            file.write(reinterpret_cast<const char*>(&entry.height), 1) != 1 ||
            file.write(reinterpret_cast<const char*>(&entry.colorCount), 1) != 1 ||
            file.write(reinterpret_cast<const char*>(&entry.reserved), 1) != 1 ||
            !write16(entry.planes) ||
            !write16(entry.bitCount) ||
            !write32(entry.size) ||
            !write32(entry.offset)) {
            return false;
        }
    }

    for (const IcoImage& image : images) {
        if (file.write(image.data) != image.data.size())
            return false;
    }

    return true;
}

bool FolderIconInstaller::writeDesktopIni(const QString& folderPath) const
{
    const QString path = QDir(folderPath).filePath(DesktopIniName);

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    const QString contents =
        QStringLiteral("[.ShellClassInfo]\r\n"
                       "IconFile=Iconizer.ico\r\n"
                       "IconIndex=0\r\n");

    QByteArray data;
    data.append("\xFF\xFE", 2);
    data.append(reinterpret_cast<const char*>(contents.utf16()),
                contents.size() * sizeof(QChar));

    return file.write(data) == data.size();
}
bool FolderIconInstaller::setWindowsAttributes(const QString& folderPath) const
{
#ifdef Q_OS_WIN
    const QString desktopIniPath = QDir(folderPath).filePath(DesktopIniName);
    const QString iconPath = QDir(folderPath).filePath(IconFileName);

    const DWORD folderAttributes =
        FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_SYSTEM;

    const DWORD fileAttributes =
        FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

    if (!SetFileAttributesW(reinterpret_cast<LPCWSTR>(folderPath.utf16()),
                            folderAttributes)) {
        return false;
    }

    if (!SetFileAttributesW(reinterpret_cast<LPCWSTR>(desktopIniPath.utf16()),
                            fileAttributes)) {
        return false;
    }

    if (!SetFileAttributesW(reinterpret_cast<LPCWSTR>(iconPath.utf16()),
                            fileAttributes)) {
        return false;
    }

    return true;
#else
    Q_UNUSED(folderPath)
    return false;
#endif
}

void FolderIconInstaller::notifyShell(const QString& folderPath) const
{
#ifdef Q_OS_WIN
    SHChangeNotify(SHCNE_UPDATEDIR,
                   SHCNF_PATHW,
                   reinterpret_cast<LPCWSTR>(folderPath.utf16()),
                   nullptr);

    SHChangeNotify(SHCNE_UPDATEITEM,
                   SHCNF_PATHW,
                   reinterpret_cast<LPCWSTR>(folderPath.utf16()),
                   nullptr);
#else
    Q_UNUSED(folderPath)
#endif
}

bool FolderIconInstaller::fail(const QString& error)
{
    emit installationError(error);
    return false;
}