#include "androidutil.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

static const QString APPNAME("de.wheeland.foos");

static bool assertDirCreated(const QString &top, const QString &name)
{
    const QDir topDir(top);
    if (!topDir.exists()) {
        qWarning() << topDir.absolutePath() << "doesn't exist";
        return false;
    }

    const QFileInfo subDir(topDir, name);
    if (subDir.exists() && subDir.isDir())
        return true;

    const bool create = topDir.mkdir(name);
    if (!create)
        qWarning() << "Can't create" << topDir.absolutePath() << name;
    return create;
}

bool AndroidUtil::createAppStorageDir()
{
    const QString storagePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    return assertDirCreated(storagePath, "data")
            && assertDirCreated(storagePath + "/data/", APPNAME);
}

QString AndroidUtil::appStorageDirPath()
{
    const QString storagePath = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation);

    return storagePath + "/data/" + APPNAME + "/";
}
