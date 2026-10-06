#include "iconmodel.h"

IconModel::IconModel(QObject* parent)
    : QAbstractListModel(parent)
{
    // Встроенные иконки входят в ресурсы приложения.
    m_icons = {
        {
            QStringLiteral("Папка"),
            QStringLiteral("qrc:/icons/folder.svg"),
            true
        },
        {
            QStringLiteral("Документы"),
            QStringLiteral("qrc:/icons/documents.svg"),
            true
        },
        {
            QStringLiteral("Игры"),
            QStringLiteral("qrc:/icons/games.svg"),
            true
        },
        {
            QStringLiteral("Музыка"),
            QStringLiteral("qrc:/icons/music.svg"),
            true
        },
        {
            QStringLiteral("Изображения"),
            QStringLiteral("qrc:/icons/pictures.svg"),
            true
        }
    };
}

int IconModel::rowCount(const QModelIndex& parent) const
{
    // У списка нет дочерних элементов.
    if (parent.isValid())
        return 0;

    return m_icons.size();
}

QVariant IconModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_icons.size())
        return {};

    const Icon& icon = m_icons.at(index.row());

    switch (role) {
        case NameRole:
            return icon.name;

        case SourceRole:
            return icon.source;

        case BuiltinRole:
            return icon.builtin;

        default:
            return {};
    }
}

QHash<int, QByteArray> IconModel::roleNames() const
{
    return {
        {NameRole, "name"},
        {SourceRole, "source"},
        {BuiltinRole, "builtin"}
    };
}

void IconModel::addIcon(const Icon& icon)
{
    const int row = m_icons.size();

    beginInsertRows(QModelIndex(), row, row);
    m_icons.append(icon);
    endInsertRows();
}