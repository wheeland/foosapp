#!/bin/sh

QT_PATH=/opt/Qt/5.12.5-android

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QT_PATH/lib
CMAKE_PREFIX_PATH=$QT_PATH
QT_PLUGIN_PATH=$QT_PATH/plugins
PATH=$QT_PATH/bin:$PATH

export ANDROID_NDK_ROOT=/home/hagen/software/android/android-ndk-r20
export ANDROID_SDK_ROOT=/home/hagen/software/android/sdk-tools

qmake .. CONFIG+=release

make -j9
make install INSTALL_ROOT=`pwd`

androiddeployqt \
    --input android-libfoosapp.so-deployment-settings.json \
    --output `pwd` \
    --gradle \
    --android-platform android-23 \
#    --release

cp build/outputs/apk/*/build-android-release-unsigned.apk foosapp.apk

$ANDROID_SDK_ROOT/platform-tools/adb install foosapp.apk
