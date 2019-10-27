#include "datamodel.h"

#include <QDataStream>

bool operator==(const DataModel::Player &lhs, const DataModel::Player &rhs)
{
    return lhs.firstName == rhs.firstName
            && lhs.lastName == rhs.lastName;
}

QDataStream &operator<<(QDataStream &stream, const DataModel::Player &player)
{
    stream << player.firstName;
    stream << player.lastName;
    stream << player.lastUpdate;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DataModel::Player &player)
{
    stream >> player.firstName;
    stream >> player.lastName;
    stream >> player.lastUpdate;
    return stream;
}

QDataStream &operator<<(QDataStream &stream, const DataModel::Model_V0 &model)
{
    stream << model.players;
    stream << model.myself;
    stream << model.training;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DataModel::Model_V0 &model)
{
    stream >> model.players;
    stream >> model.myself;
    stream >> model.training;
    return stream;
}

namespace DataModel
{

int qHash(const Player &player)
{
    return qHash(player.firstName) ^ qHash(player.lastName);
}

QByteArray Model_V0::toString() const
{
    QByteArray data;
    {
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << *this;
    }
    return data.toBase64();
}

bool Model_V0::fromString(const QByteArray &string)
{
    const QByteArray data = QByteArray::fromBase64(string);
    QDataStream stream(data);
    stream >> *this;
    return (stream.status() == QDataStream::Ok);
}

bool Model_V0::operator==(const Model_V0 &other) const
{
    return (players == other.players)
            && (myself == other.myself)
            && (training == other.training);
}

QString randWord()
{
    QString ret;
    const int len = 2 + qrand() % 6;
    for (int j = 0; j < len; ++j)
        ret += ('a' + qrand() % 26);
    return ret;
}

QString randStr(int len)
{
    QString ret;
    for (int i = 0; i < len; ++i) {
        ret += randWord();
        ret += (qrand() % 10 < 3) ? '\n' : ' ';
    }
    return ret;
}

Notes dummyNotes()
{
    Notes ret;
    ret << QPair<int, QString>(0x030102, randStr(30));
    ret << QPair<int, QString>(0x030102, randStr(40));
    ret << QPair<int, QString>(0x040103, randStr(50));
    ret << QPair<int, QString>(0x010203, randStr(60));
    return ret;
}

Model_V0 dummy()
{
    Model_V0 ret;
    ret.myself = dummyNotes();
    ret.training = dummyNotes();
    for (int i = 0; i < 20; ++i) {
        QString first = randWord();
        QString last = randWord();
        Player player{ first, last, QDateTime::currentDateTime() };
        ret.players[player] = dummyNotes();
    }
    return ret;
}

} //namespace DataModel
