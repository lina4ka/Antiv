QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

unix|win32: LIBS += -L$$PWD/../build-BaseWork-Desktop_Qt_6_0_3_MSVC2019_64bit-Debug/debug/ -lBaseWork

INCLUDEPATH += $$PWD/../BaseWork
DEPENDPATH += $$PWD/../BaseWork

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/../build-BaseWork-Desktop_Qt_6_0_3_MSVC2019_64bit-Debug/debug/BaseWork.lib
else:unix|win32-g++: PRE_TARGETDEPS += $$PWD/../build-BaseWork-Desktop_Qt_6_0_3_MSVC2019_64bit-Debug/debug/libBaseWork.a
