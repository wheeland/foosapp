#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QScreen>

#include <QFile>
#include <QDir>

#include "androidutil.h"
#include "qmlmodel.h"
#include "controller.h"
#include "logger.h"

static QString DEFAULT_PATH = QString("data.bin");

static QString getDateString(const QDateTime &dt)
{
    return QString::asprintf("%d_%d_%d_%d_%d_%d.bin",
                             dt.date().year(),
                             dt.date().month(),
                             dt.date().day(),
                             dt.time().hour(),
                             dt.time().minute(),
                             dt.time().second());
}

DataModel::Model_V0 loadModel(const QString &path)
{
    DataModel::Model_V0 ret;

    QFile data(path);
    if (!data.exists()) {
        qWarning() << "loadModel: file doesn't exist:" << path;
        return ret;
    }
    if (!data.open(QIODevice::ReadOnly)) {
        qWarning() << "loadModel: can't open for reading:" << path;
        return ret;
    }
    ret.fromString(data.readAll());

    return ret;
}

void saveModel(const DataModel::Model_V0 &model, const QString &path)
{
    QFile data(path);
    if (!data.open(QIODevice::WriteOnly)) {
        qWarning() << "saveModel: can't open for writing:" << path;
        return;
    }
    data.write(model.toString());
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Logger logger;
#ifdef Q_OS_ANDROID
    logger.install();
#endif

    // load data from storage
    const bool created = AndroidUtil::createAppStorageDir();
    qWarning() << "AndroidUtil::createAppStorageDir():" << created;
    qWarning() << "dir =" << AndroidUtil::appStorageDirPath();
    const QString dataFileName = AndroidUtil::appStorageDirPath() + DEFAULT_PATH;
    qWarning() << "file =" << dataFileName;
    qWarning() << "permissions =" << QFile::permissions(dataFileName);
    DataModel::Model_V0 model = loadModel(dataFileName);
    Database database(model);
    Controller controller(&database);

    qmlRegisterType<Category>("Foos", 1, 0, "Category");
    qmlRegisterType<Note>("Foos", 1, 0, "Note");
    qmlRegisterType<Player>("Foos", 1, 0, "Player");
    qmlRegisterType<NotesSortModel>("Foos", 1, 0, "NotesSortModel");
    qmlRegisterUncreatableType<Database>("Foos", 1, 0, "Database", "nope.");
    qmlRegisterUncreatableType<Controller>("Foos", 1, 0, "Controller", "nope.");

#ifdef Q_OS_ANDROID
    const QSize screenSize = app.primaryScreen()->size();
#else
    const QSize screenSize = QSize(540, 920);
#endif

    QQuickView view;
    view.resize(screenSize);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("_controller", &controller);
    view.rootContext()->setContextProperty("_logger", &logger);
    view.rootContext()->setContextProperty("_scale", screenSize.width() / 240.0f);

    QQmlComponent styleComponent(view.engine(), "qrc:/qml/Style.qml");
    QObject *styleObject = styleComponent.create(view.rootContext());
    view.rootContext()->setContextProperty("_style", styleObject);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    const int ret = app.exec();

    const DataModel::Model_V0 outModel = database.toModel_V0();
    if (outModel != model) {
        saveModel(outModel, AndroidUtil::appStorageDirPath() + DEFAULT_PATH);
        saveModel(outModel, AndroidUtil::appStorageDirPath() + getDateString(QDateTime::currentDateTime()) + ".bin");
    }

    return ret;
}
