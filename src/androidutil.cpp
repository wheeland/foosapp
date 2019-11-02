#include "androidutil.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QFileInfo>

static const QString APPNAME("de.wheeland.foos");

static bool assertDirCreated(const QString &top, const QString &name)
{
    const QDir topDir(top);
    if (!topDir.exists())
        return false;

    const QFileInfo subDir(topDir, name);
    if (subDir.exists() && subDir.isDir())
        return true;

    return topDir.mkdir(name);
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
