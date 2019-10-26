#pragma once

#include <QObject>
#include "datamodel.h"

class App : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    ~App();

private:
    DataModel::Model_V0 m_dataModel;
};
