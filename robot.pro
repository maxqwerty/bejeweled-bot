QT       += core gui

DESTDIR  = $$PWD/../bin
TARGET   = bejeweled_bot
TEMPLATE = app


SOURCES += main.cpp
SOURCES += mainwindow.cpp
SOURCES += solver.cpp
SOURCES += helper.cpp
SOURCES += aboutdlg.cpp

HEADERS += mainwindow.h
HEADERS += solver.h
HEADERS += helper.h
HEADERS += aboutdlg.h

FORMS   += mainwindow.ui
FORMS   += aboutdlg.ui

win32 {
  LIBS += -lgdi32
  RC_FILE = res/res.rc
}

RESOURCES += res.qrc

include($$PWD/globalshortcut/globalshortcut.pri)

