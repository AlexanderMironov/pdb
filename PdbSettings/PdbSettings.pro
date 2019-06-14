#-------------------------------------------------
#
# Project created by QtCreator 2012-06-01T10:25:11
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += sql
QT       += xml

CONFIG 	 += plugin
CONFIG 	 += crypto
CONFIG += static
#/usr/lib/x86_64-linux-gnu/
#/usr/lib/x86_64-linux-gnu/
#/home/alex/Downloads/qca-master/lib/      libqca-qt5.so
#LIBS	 +=  -L/home/alex/Downloads/qca-master/lib/  -lqca-qt5
#LIBS	 +=  -L/home/alex/Downloads/qca-master/lib/qca/crypto -lqca-ossl
LIBS	 += -lqca-qt5
#LIBS	 += -lqca

TARGET = PdbSettings
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    colorgraficelements.cpp \
    nodesandattachmentsgraficeelements.cpp \
    abstractgraficeelements.cpp \
    dbgraficelements.cpp \
    loggetelements.cpp \
    securityelements.cpp \
    dbcreator.cpp \
    dbstructuredescriptor.cpp \
    ../CommonInclude/pdb/DBSettings.cpp \
    ../CommonInclude/pdb/cryptosupport.cpp \
    scannersettings.cpp

HEADERS  += mainwindow.h \
    dbgraficelements.h \
    colorgraficelements.h \
    ../CommonInclude/pdb/pdb_style.h \
    nodesandattachmentsgraficeelements.h \
    abstractgraficeelements.h \
    SectionNames.h \
    loggetelements.h \
    securityelements.h \
    dbcreator.h \
    dbstructuredescriptor.h \
    ../CommonInclude/pdb/DBSettings.h \
    ../CommonInclude/pdb/DBSettingsStream.h \
    ../CommonInclude/pdb/cryptosupport.h \
    ../CommonInclude/pdb/VariantPtr.h \
    scannersettings.h

FORMS    += mainwindow.ui

RESOURCES += \
    PdbSettings.qrc
