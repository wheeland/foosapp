#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <QQmlEngine>
#include <QQmlComponent>

#include "qmlmodel.h"
#include "controller.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    DataModel::Model_V0 model = DataModel::dummy();
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

    return app.exec();
}
