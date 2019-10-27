#pragma once

#include <QAbstractListModel>

struct MenuButton
{
    QString text;
    float center;
};

class MenuModel : public QAbstractListModel
{
    Q_OBJECT

    enum Role {
        TextRole = Qt::UserRole + 1,
        PositionRole
    };

public:
    using QAbstractListModel::QAbstractListModel;
    ~MenuModel() override = default;

    int rowCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void setButtons(const QVector<MenuButton> &buttons);

private:
    QVector<MenuButton> m_buttons;
};
