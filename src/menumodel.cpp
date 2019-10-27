#include "menumodel.h"
#include <QDebug>

int MenuModel::rowCount(const QModelIndex &parent) const
{
    return m_buttons.size();
}

QVariant MenuModel::data(const QModelIndex &index, int role) const
{
    const int idx = index.row();
    if (idx < 0 || idx >= m_buttons.size()) {
        qWarning() << "MenuModel: invalid index" << idx;
        return QVariant();
    }

    switch (role) {
    case TextRole:
        return QVariant::fromValue(m_buttons[idx].text);
    case PositionRole:
        return QVariant::fromValue(m_buttons[idx].center);
    default:
        return QVariant();
    }
}

QHash<int, QByteArray> MenuModel::roleNames() const
{
    QHash<int, QByteArray> ret;
    ret[TextRole] = "text";
    ret[PositionRole] = "position";
    return ret;
}

void MenuModel::setButtons(const QVector<MenuButton> &buttons)
{
    beginResetModel();
    m_buttons = buttons;
    endResetModel();
}
