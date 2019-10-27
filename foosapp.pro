TARGET = foosapp

TEMPLATE = app

QT += qml quick

SOURCES += \
    src/main.cpp \
    src/app.cpp \
    src/qmlmodel.cpp \
    src/datamodel.cpp \

HEADERS += \
    src/app.h \
    src/qmlmodel.h \
    src/datamodel.h \

RESOURCES += \
    qml.qrc
