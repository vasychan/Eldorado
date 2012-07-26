#-------------------------------------------------
#
# Project created by QtCreator 2012-07-17T13:38:19
#
#-------------------------------------------------

QT       += core gui phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eldorado
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lib/httpclient.cpp \
    lib/playlist.cpp \
    lib/parser.cpp \
    lib/stream.cpp

HEADERS  += mainwindow.h \
    lib/httpclient.h \
    configure.h \
    lib/playlist.h \
    lib/parser.h \
    lib/stream.h

FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lboost_system
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lboost_system
else:symbian: LIBS += -lboost_system
else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../../usr/include/boost
DEPENDPATH += $$PWD/../../../../usr/include/boost

win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/release/boost_system.lib
else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/debug/boost_system.lib
else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_system.a
