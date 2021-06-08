QT += testlib
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_zipfiletest.cpp

include($$PWD/../../ZipFile.pri)

HEADERS += \
    tst_zipfiletest.h

DEFINES += PATH=\"\\\"$$OUT_PWD\\\"\"
