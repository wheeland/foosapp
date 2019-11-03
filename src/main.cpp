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
#include "app.h"

class CloseFilter : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    ~CloseFilter() override = default;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::Close) {
            emit backButtonPressed();
            return true;
        }
        return false;
    }

signals:
    void backButtonPressed();
};

static const QString DEFAULT_PATH = QString("data.bin");

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

    // Prepare storage
    const QString appStorageDirPath = AndroidUtil::appStorageDirPath();
    if (!AndroidUtil::createAppStorageDir())
        qWarning() << "Failed to create app storage directory";

    // load data from storage
    const QString dataFileName = appStorageDirPath + DEFAULT_PATH;
    DataModel::Model_V0 model = loadModel(dataFileName);

    Database database(model);
    Controller controller(&database);

    // React to close signal
    CloseFilter filter;
    app.installEventFilter(&filter);
    QObject::connect(&filter, &CloseFilter::backButtonPressed, [&]() {
        if (!controller.onBackButton())
            app.exit();
    });

    // save data to storage, whenever something changed
    QObject::connect(&controller, &Controller::saveData, [&]() {
        const DataModel::Model_V0 outModel = database.toModel_V0();
        saveModel(outModel, dataFileName);

        // we want to save a backup every day
        const QDate dt = QDate::currentDate();
        const QString dateStr = QString::asprintf("%d_%02d_%02d", dt.year(), dt.month(), dt.day());
        const QString backupFilePath = appStorageDirPath + dateStr + ".bin";
        if (!QFile(backupFilePath).exists()) {
            saveModel(outModel, backupFilePath);
        }
    });

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

#ifdef Q_OS_ANDROID
    logger.uninstall();
#endif

    return ret;
}

#include "main.moc"
