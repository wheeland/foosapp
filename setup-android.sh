#!/bin/sh

QT_PATH=/opt/Qt/5.12.5-android

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$QT_PATH/lib
CMAKE_PREFIX_PATH=$QT_PATH
QT_PLUGIN_PATH=$QT_PATH/plugins
PATH=$QT_PATH/bin:$PATH

export ANDROID_NDK_ROOT=/home/hagen/software/android/android-ndk-r20
export ANDROID_SDK_ROOT=/home/hagen/software/android/sdktools

qmake ..
