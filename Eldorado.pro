#-------------------------------------------------
#
# Project created by QtCreator 2012-07-17T13:38:19
#
#-------------------------------------------------

QT += core
QT += gui4
QT += phonon

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Eldorado
TEMPLATE = app

CONFIG += -fno-exceptions
CONFIG += -static
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
win32: LIBS += -LC:/Qt/4.8.2/lib/ -lQtCore4

INCLUDEPATH += C:/Qt/4.8.2/include
DEPENDPATH += C:/Qt/4.8.2/include

win32: PRE_TARGETDEPS += C:/Qt/4.8.2/lib/QtCore4.lib

win32: LIBS += -LC:/Qt/4.8.2/lib/ -lQtGui4

INCLUDEPATH += C:/Qt/4.8.2/include
DEPENDPATH += C:/Qt/4.8.2/include

win32: PRE_TARGETDEPS += C:/Qt/4.8.2/lib/QtGui4.lib
win32: LIBS += -LC:/Qt/4.8.2/lib/ -lphonon4

INCLUDEPATH += C:/Qt/4.8.2/include
DEPENDPATH += C:/Qt/4.8.2/include

win32: PRE_TARGETDEPS += C:/Qt/4.8.2/lib/phonon4.lib
DEPLOYMENT_PLUGIN += phonon_ds9 phonon_waveout
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/lib/release/ -lboost_system
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/lib/debug/ -lboost_system
#else:symbian: LIBS += -lboost_system
#else:unix: LIBS += -L$$PWD/../../../../usr/lib/ -lboost_system

INCLUDEPATH += $$PWD/../../../../usr/include/boost
DEPENDPATH += $$PWD/../../../../usr/include/boost
INCLUDEPATH += D:\projects\boost_1_50_0
DEPENDPATH += D:\projects\boost_1_50_0
#win32:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/release/boost_system.lib
#else:win32:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/lib/debug/boost_system.lib
#else:unix:!symbian: PRE_TARGETDEPS += $$PWD/../../../../usr/lib/libboost_system.a

RESOURCES += \
    res/icons.qrc



win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/release/link-static/threading-multi/ -llibboost_system-vc90-mt-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/release/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/release/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/release/link-static/threading-multi/ -llibboost_date_time-vc90-mt-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/release/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/release/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/release/link-static/threading-multi/ -llibboost_regex-vc90-mt-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/release/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/release/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/release/link-static/threading-multi/ -llibboost_thread-vc90-mt-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/release/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/release/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/release/link-static/threading-multi/ -llibboost_chrono-vc90-mt-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/release/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/release/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/debug/link-static/threading-multi/ -llibboost_date_time-vc90-mt-gd-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/debug/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/date_time/build/msvc-9.0/debug/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/debug/link-static/threading-multi/ -llibboost_regex-vc90-mt-gd-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/debug/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/regex/build/msvc-9.0/debug/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/debug/link-static/threading-multi/ -llibboost_thread-vc90-mt-gd-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/debug/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/thread/build/msvc-9.0/debug/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/debug/link-static/threading-multi/ -llibboost_chrono-vc90-mt-gd-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/debug/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/chrono/build/msvc-9.0/debug/link-static/threading-multi

win32: LIBS += -L$$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/debug/link-static/threading-multi/ -llibboost_system-vc90-mt-gd-1_50

INCLUDEPATH += $$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/debug/link-static/threading-multi
DEPENDPATH += $$PWD/../boost_1_50_0/bin.v2/libs/system/build/msvc-9.0/debug/link-static/threading-multi




