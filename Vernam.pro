QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    history.cpp \
    historydialog.cpp \
    main.cpp \
    mainwindow.cpp \
    vernam.c

HEADERS += \
    history.h \
    historydialog.h \
    mainwindow.h \
    vernam.h

FORMS += \
    historydialog.ui \
    mainwindow.ui

TRANSLATIONS += \
    translations/fr_FR.ts \
    translations/en_EN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle.properties \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    tranalations/en_EN.ts

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}

