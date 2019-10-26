#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>

#include "controller.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    DataModel::Model_V0 model = DataModel::dummy();
    FoosController controller(model);

    qmlRegisterType<Category>("Foos", 1, 0, "Category");
    qmlRegisterType<Note>("Foos", 1, 0, "Note");
    qmlRegisterType<Player>("Foos", 1, 0, "Player");
    qmlRegisterUncreatableType<FoosController>("Foos", 1, 0, "Controller", "nope.");

    QQuickView view;
    view.resize(540, 960);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.rootContext()->setContextProperty("_controller", &controller);
    view.rootContext()->setContextProperty("_scale", 2.0f);
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    return app.exec();
}
