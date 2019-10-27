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

bool operator==(const Player &lhs, const Player &rhs);
int qHash(const Player &player);

struct Model_V0
{
    QHash<Player, Notes> players;
    Notes myself;
    Notes training;

    QByteArray toString() const;
    bool fromString(const QByteArray &string);

    bool operator==(const Model_V0 &other) const;
    bool operator!=(const Model_V0 &other) const { return !(*this == other); }
};

Model_V0 dummy();

} //namespace DataModel
