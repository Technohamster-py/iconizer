#pragma once

#include <QSortFilterProxyModel>

/**
 * @brief Прокси-модель для фильтрации иконок по типу.
 *
 * Позволяет получить из IconModel только встроенные
 * или только пользовательские иконки.
 */
class IconFilterModel final : public QSortFilterProxyModel
{
    Q_OBJECT

    Q_PROPERTY(bool builtin READ builtin WRITE setBuiltin NOTIFY builtinChanged)

public:
    explicit IconFilterModel(QObject* parent = nullptr);

    /**
     * @brief Возвращает тип иконок, которые должна содержать модель.
     *
     * @return true для встроенных иконок, false для пользовательских.
     */
    [[nodiscard]] bool builtin() const;

    /**
     * @brief Устанавливает тип иконок для фильтрации.
     *
     * @param builtin true для встроенных иконок, false для пользовательских.
     */
    void setBuiltin(bool builtin);

    signals:
        /**
         * @brief Сигнал изменения типа фильтра.
         */
        void builtinChanged();

protected:
    /**
     * @brief Проверяет, должна ли строка исходной модели попасть в прокси-модель.
     */
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    bool m_builtin = true;
};