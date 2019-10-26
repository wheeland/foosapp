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

QString modelToString(const Model_V0 &model)
{
    QByteArray data;
    {
        QDataStream stream(&data, QIODevice::WriteOnly);
        stream << model;
    }
    return QString(data.toBase64());
}

Model_V0 stringToModel(const QString &string)
{
    const QByteArray data = QByteArray::fromBase64(string.toLocal8Bit());
    QDataStream stream(data);
    Model_V0 out;
    stream >> out;
    return out;
}

QString randStr(int len)
{
    QString ret;
    for (int i = 0; i < len; ) {
        const int len = 2 + qrand() % 6;
        for (int j = 0; j < len; ++j)
            ret += ('a' + qrand() % 26);
        ret += (qrand() % 10 < 3) ? '\n' : ' ';
        i += len;
    }
    return ret;
}

Model_V0 dummy()
{
    Model_V0 ret;
    ret.myself << QPair<int, QString>(0x030102, randStr(100));
    ret.myself << QPair<int, QString>(0x030102, randStr(200));
    ret.myself << QPair<int, QString>(0x040103, randStr(300));
    ret.myself << QPair<int, QString>(0x010203, randStr(400));
    return ret;
}

} //namespace DataModel
