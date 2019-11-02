TARGET = foosapp

TEMPLATE = app

QT += qml quick svg

SOURCES += \
    src/main.cpp \
    src/androidutil.cpp \
    src/app.cpp \
    src/qmlmodel.cpp \
    src/menumodel.cpp \
    src/controller.cpp \
    src/datamodel.cpp \
    src/logger.cpp \

HEADERS += \
    src/androidutil.h \
    src/app.h \
    src/qmlmodel.h \
    src/menumodel.h \
    src/controller.h \
    src/datamodel.h \
    src/logger.h \

RESOURCES += \
    qml.qrc

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    DISTFILES += \
        android/AndroidManifest.xml \
        android/gradle/wrapper/gradle-wrapper.jar \
        android/gradlew \
        android/res/values/libs.xml \
        android/build.gradle \
        android/gradle/wrapper/gradle-wrapper.properties \
        android/gradlew.bat

    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
