#include "datamodel.h"

#include <QDataStream>

QDataStream &operator<<(QDataStream &stream, const DataModel::Model_V0 &model)
{
    stream << model.players;
    stream << model.training;
    stream << model.strategy;
    return stream;
}

QDataStream &operator>>(QDataStream &stream, DataModel::Model_V0 &model)
{
    stream >> model.players;
    stream >> model.training;
    stream >> model.strategy;
    return stream;
}

namespace DataModel
{

QString modelToString(const Model_V0 &model)
{
    QByteArray data;
    {
        QDataStream stream(&data);
        stream << model;
    }
    return QString(data.toBase64());
}

Model_V0 stringToModel(const QString &string)
{
    const QByteArray data = QByteArray::fromBase64(string.toLocal8Bit());
    QDataStream stream(&data);
    Model_V0 out;
    stream >> out;
    return out;

}

} //namespace DataModel
