#include "iconfiltermodel.h"

#include "iconmodel.h"

IconFilterModel::IconFilterModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
}

bool IconFilterModel::builtin() const
{
    return m_builtin;
}

void IconFilterModel::setBuiltin(bool builtin)
{
    if (m_builtin == builtin)
        return;

    m_builtin = builtin;
    invalidateFilter();
    emit builtinChanged();
}

bool IconFilterModel::filterAcceptsRow(
    int sourceRow,
    const QModelIndex& sourceParent) const
{
    const QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);

    return index.data(IconModel::BuiltinRole).toBool() == m_builtin;
}
