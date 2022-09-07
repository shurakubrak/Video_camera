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
    main.cpp \
    metadatadialog.cpp \
    settings.cpp \
    sqlite-amalgama/shell.c \
    sqlite-amalgama/sqlite3.c \
    sqlite_db.cpp \
    video.cpp

HEADERS += \
    metadatadialog.h \
    settings.h \
    sqlite-amalgama/sqlite3.h \
    sqlite-amalgama/sqlite3ext.h \
    sqlite_db.h \
    video.h

FORMS += \
    video.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

QT += widgets
include(C:/Qt/Examples/Qt-6.3.1/multimedia/shared/shared.pri)

DISTFILES +=

RESOURCES += \
    Images/video.qrc
