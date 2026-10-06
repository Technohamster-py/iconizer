#pragma once

#include <QAbstractListModel>
#include <QVector>

/**
 * @brief Описание одной иконки.
 */
struct Icon {
    QString name;
    QString source;
    bool builtin = false;
};

/**
 * @brief Модель библиотеки иконок для QML.
 *
 * Содержит как встроенные, так и пользовательские иконки.
 */
class IconModel final : public QAbstractListModel
{
    Q_OBJECT

public:
    /**
     * @brief Роли, доступные из QML.
     */
    enum Roles {
        NameRole = Qt::UserRole + 1,
        SourceRole,
        BuiltinRole
    };

    explicit IconModel(QObject* parent = nullptr);

    /**
     * @brief Возвращает количество иконок.
     */
    int rowCount(const QModelIndex& parent = {}) const override;

    /**
     * @brief Возвращает значение роли для указанной иконки.
     */
    QVariant data(
        const QModelIndex& index,
        int role = Qt::DisplayRole) const override;

    /**
     * @brief Возвращает имена ролей модели.
     */
    QHash<int, QByteArray> roleNames() const override;

    /**
     * @brief Добавляет иконку в модель.
     */
    void addIcon(const Icon& icon);

private:
    QVector<Icon> m_icons;
};