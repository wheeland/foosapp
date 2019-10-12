#pragma once

#include <QString>
#include <QHash>
#include <QVector>

namespace DataModel
{

using Notes = QHash<QString, QString>;

struct Model_V0
{
    QHash<QString, Notes> players;
    Notes training;
    Notes strategy;
};

QString modelToString(const Model_V0 &model);
Model_V0 stringToModel(const QString &string);

} //namespace DataModel
