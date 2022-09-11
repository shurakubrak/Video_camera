TEMPLATE = app
TARGET = video

QT  += core gui
QT  += multimedia multimediawidgets

#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    anchor.cpp \
    danchors.cpp \
    dtable.cpp \
    main.cpp \
    metadatadialog.cpp \
    qtableviewmodel.cpp \
    settings.cpp \
    sqlite-amalgama/shell.c \
    sqlite-amalgama/sqlite3.c \
    sqlite_db.cpp \
    video.cpp

HEADERS += \
    anchor.h \
    danchors.h \
    dtable.h \
    metadatadialog.h \
    qtableviewmodel.h \
    settings.h \
    sqlite-amalgama/sqlite3.h \
    sqlite-amalgama/sqlite3ext.h \
    sqlite_db.h \
    video.h

FORMS += \
    danchors.ui \
    dtable.ui \
    video.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += widgets
include(C:/Qt/Examples/Qt-6.3.1/multimedia/shared/shared.pri)

QT += sql

DISTFILES +=

RESOURCES += \
    Images/video.qrc



win32: LIBS += -L$$PWD/pgsql/lib/ -llibpq

#INCLUDEPATH += $$PWD/pgsql/include
DEPENDPATH += $$PWD/pgsql/lib

#win32:!win32-g++: PRE_TARGETDEPS += $$PWD/pgsql/lib/libpq.lib
#else:win32-g++: PRE_TARGETDEPS += $$PWD/pgsql/lib/liblibpq.a
