#include <QGuiApplication>
#include <QQuickView>
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QQuickView view;
    view.resize(920, 480);
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setSource(QUrl("qrc:/qml/main.qml"));
    view.show();

    return app.exec();
}
