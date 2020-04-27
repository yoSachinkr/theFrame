QT += widgets thelib
SHARE_APP_NAME=theframe/libtheframe

TEMPLATE = lib
DEFINES += LIBTHEFRAME_LIBRARY
TARGET = theframe

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    elements/element.cpp \
    elements/rectangleelement.cpp \
    elements/timelineelement.cpp \
    elements/viewportelement.cpp

HEADERS += \
    elements/element.h \
    elements/rectangleelement.h \
    elements/timelineelement.h \
    elements/viewportelement.h \
    libtheframe_global.h

unix:!macx {
    # Include the-libs build tools
    include(/usr/share/the-libs/pri/buildmaster.pri)

    TARGET = theframe
    target.path = /usr/lib

    translations.path = /usr/share/theframe/libtheframe/translations
    translations.files = translations/*

    INSTALLS += target translations
}
!isEmpty(target.path): INSTALLS += target
