#pragma once

#include <QObject>

/**
 * @brief Устанавливает пользовательские иконки для папок Windows.
 *
 * Использует desktop.ini и отдельный Iconizer.ico внутри целевой папки.
 */
class FolderIconInstaller final : public QObject
{
    Q_OBJECT

public:
    explicit FolderIconInstaller(QObject* parent = nullptr);

    /**
     * @brief Устанавливает иконку для папки.
     *
     * @param folderPath Путь к целевой папке.
     * @param iconPath Путь к ICO или SVG-файлу.
     * @return true, если иконка успешно установлена.
     */
    Q_INVOKABLE bool installIcon(const QString& folderPath,
                                 const QString& iconPath);

    /**
     * @brief Удаляет иконку Iconizer с папки.
     *
     * Если до установки Iconizer существовал desktop.ini,
     * он восстанавливается из резервной копии.
     *
     * @param folderPath Путь к папке.
     * @return true, если операция успешно выполнена.
     */
    Q_INVOKABLE bool removeIcon(const QString& folderPath);

    /**
     * @brief Проверяет наличие установленной Iconizer иконки.
     *
     * @param folderPath Путь к папке.
     * @return true, если папка содержит desktop.ini, созданный Iconizer.
     */
    Q_INVOKABLE bool hasCustomIcon(const QString& folderPath) const;

signals:
    /**
     * @brief Сигнал об ошибке установки или удаления иконки.
     *
     * @param error Текст ошибки.
     */
    void installationError(const QString& error);

private:
    /**
     * @brief Создаёт ICO-файл из исходного изображения.
     */
    bool createIco(const QString& sourcePath, const QString& destinationPath);

    /**
     * @brief Загружает исходное изображение и преобразует его в QImage.
     */
    QImage loadImage(const QString& sourcePath) const;

    /**
     * @brief Сохраняет QImage как Windows ICO с несколькими размерами.
     */
    bool writeIco(const QImage& image, const QString& destinationPath) const;

    /**
     * @brief Записывает desktop.ini для Iconizer.
     */
    bool writeDesktopIni(const QString& folderPath) const;

    /**
     * @brief Устанавливает Windows-атрибуты для папки и служебных файлов.
     */
    bool setWindowsAttributes(const QString& folderPath) const;

    /**
     * @brief Уведомляет Windows Explorer об изменении папки.
     */
    void notifyShell(const QString& folderPath) const;

    /**
     * @brief Устанавливает текст последней ошибки и испускает сигнал.
     */
    bool fail(const QString& error);
};