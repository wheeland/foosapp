#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>

#include <QFile>
#include <QDir>

#include "qmlmodel.h"
#include "controller.h"

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
    if (data.exists() && data.open(QIODevice::ReadOnly)) {
        ret.fromString(data.readAll());
    }

    return ret;
}

void saveModel(const DataModel::Model_V0 &model, const QString &path)
{
    QFile data(path);
    if (data.open(QIODevice::WriteOnly)) {
        data.write(model.toString());
    }
}

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    DataModel::Model_V0 model = loadModel(DEFAULT_PATH);
    Database database(model);
    Controller controller(&database);

    qmlRegisterType<Category>("Foos", 1, 0, "Category");
    qmlRegisterType<Note>("Foos", 1, 0, "Note");
    qmlRegisterType<Player>("Foos", 1, 0, "Player");
    qmlRegisterType<NotesSortModel>("Foos", 1, 0, "NotesSortModel");
    qmlRegisterUncreatableType<Database>("Foos", 1, 0, "Database", "nope.");
    qmlRegisterUncreatableType<Controller>("Foos", 1, 0, "Controller", "nope.");

    QQuickView view;
    view.resize(540, 960);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("_controller", &controller);
    view.rootContext()->setContextProperty("_scale", 2.0f);

    QQmlComponent styleComponent(view.engine(), "qrc:/qml/Style.qml");
    QObject *styleObject = styleComponent.create(view.rootContext());
    view.rootContext()->setContextProperty("_style", styleObject);

    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    const int ret = app.exec();

    const DataModel::Model_V0 outModel = database.toModel_V0();
    if (outModel != model) {
        saveModel(outModel, DEFAULT_PATH);
        saveModel(outModel, getDateString(QDateTime::currentDateTime()));
    }

    return ret;
}
