CONFIG -= qt

TEMPLATE = lib
CONFIG += staticlib
CONFIG += c++17
LIBS += -lstdc++fs

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Basereader.cpp \
    Basewriter.cpp \
    Recordinfo.cpp \
    malwaredata.cpp \
    sha256.cpp

HEADERS += \
    Basereader.h \
    Basewriter.h \
    Recordinfo.h \
    malwaredata.h \
    sha256.h

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target
