QT       += core gui
TEMPLATE = app
DESTDIR  = $$PWD/../bin

PROJECT_NAME = bejeweled_bot

CONFIG(debug, release|debug) {
  TARGET      = $${PROJECT_NAME}d
  OBJECTS_DIR = $$PWD/../.tmp/debug/o
  MOC_DIR     = $$PWD/../.tmp/debug/genFiles
}

CONFIG(release, release|debug) {
  TARGET      = $${PROJECT_NAME}
  OBJECTS_DIR = $$PWD/../.tmp/release/o
  MOC_DIR     = $$PWD/../.tmp/release/genFiles
}

UI_DIR      += $$MOC_DIR
RCC_DIR     += $$MOC_DIR
INCLUDEPATH += $$MOC_DIR $$PWD/globalshortcut .

SOURCES += main.cpp

HEADERS += mainwindow.h
SOURCES += mainwindow.cpp
FORMS   += mainwindow.ui

HEADERS += solver.h
SOURCES += solver.cpp

HEADERS += helper.h
SOURCES += helper.cpp

HEADERS += aboutdlg.h
SOURCES += aboutdlg.cpp
FORMS   += aboutdlg.ui

win32 {
  LIBS += -lgdi32
  RC_FILE = res.rc
}

RESOURCES += res.qrc

include($$PWD/globalshortcut/globalshortcut.pri)

