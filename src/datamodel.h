#pragma once

#include <QString>
#include <QHash>
#include <QVector>
#include <QDateTime>

namespace DataModel
{

using Notes = QVector<QPair<int, QString>>;

struct Player
{
    QString firstName;
    QString lastName;
    QDateTime lastUpdate;
};

int qHash(const Player &player);

struct Model_V0
{
    QHash<Player, Notes> players;
    Notes myself;
    Notes training;
};

QString modelToString(const Model_V0 &model);
Model_V0 stringToModel(const QString &string);

} //namespace DataModel

bool operator==(const DataModel::Player &lhs, const DataModel::Player &rhs);
