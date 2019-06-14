#-------------------------------------------------
#
# Project created by QtCreator 2012-04-12T18:20:14
#
#-------------------------------------------------

QT       += core gui
QT       += widgets
QT       += sql
#
CONFIG   += qt
CONFIG 	 += plugin
CONFIG 	 += crypto
CONFIG   += static
QT       += printsupport
#
#LIBS	 += -L /usr/lib/x86_64-linux-gnu/ -lqca
LIBS	 += -lqca-qt5
#
#/usr/lib/x86_64-linux-gnu/
#/mnt/old_system/usr/lib/x86_64-linux-gnu/
#/usr/lib/x86_64-linux-gnu/libqca.so

TARGET = pdb
TEMPLATE = app

INCLUDE+=usr/lib/qt4/plugins/sqldrivers/
#LIBS+=-L//u01/app/oracle/product/11.2.0/lib/

SOURCES += main.cpp\
        mainwindow.cpp \
    rootoftree.cpp \
    dbacccesssafe.cpp \
    dbconnection.cpp \
    connectionmanager.cpp \
    abstractdatabaseobject.cpp \
    mytree.cpp \
    mnumainwindow.cpp \
    attachment.cpp \
    treeleaf.cpp \
    mytable.cpp \
    notificationdialog.cpp \
    globaltreeoperationshandler.cpp \
    treerenamecreatedlg.cpp \
    databasecleaner.cpp \
    attachmentclickationdialog.cpp \
    tmpfilecleaner.cpp \
    dlgexportparams.cpp \
    treestatisticdlg.cpp \
    treesearchdlg.cpp \
    servicescfg.cpp \
    logger.cpp \
    tunnelmaker.cpp \
    attachimportorreplacedlg.cpp \
    passworddlg.cpp \
    attachstatuschecker.cpp \
    askchangesdetailsdlg.cpp \
    cryptothread.cpp \
    ../CommonInclude/pdb/DBSettings.cpp \
    ../CommonInclude/pdb/cryptosupport.cpp \
    waiter.cpp \
    myeditor.cpp \
    dlginserttable.cpp \
    dlginserthtml.cpp \
    htmlsyntaxhighlighter.cpp \
    dlgaskencryptmethod.cpp \
    iconloader.cpp \
    dlgmanageicons.cpp \
    iconmanager.cpp \
    dbicon.cpp \
    dlgscanimage.cpp \
    dlgrotateparams.cpp \
    imagelabel.cpp \
    dlgsavescan.cpp \
    searchrequestres.cpp

HEADERS  += mainwindow.h \
    GDefinitions.h \
    rootoftree.h \
    dbacccesssafe.h \
    dbconnection.h \
    connectionmanager.h \
    abstractdatabaseobject.h \
    mytree.h \
    mnumainwindow.h \
    attachment.h \
    ../CommonInclude/pdb/pdb_style.h \
    treeleaf.h \
    mytable.h \
    notificationdialog.h \
    globaltreeoperationshandler.h \
    treerenamecreatedlg.h \
    databasecleaner.h \
    attachmentclickationdialog.h \
    tmpfilecleaner.h \
    dlgexportparams.h \
    treestatisticdlg.h \
    treesearchdlg.h \
    servicescfg.h \
    logger.h \
    tunnelmaker.h \
    attachimportorreplacedlg.h \
    passworddlg.h \
    attachstatuschecker.h \
    askchangesdetailsdlg.h \
    cryptothread.h \
    ../CommonInclude/pdb/DBSettingsStream.h \
    ../CommonInclude/pdb/DBSettings.h \
    ../CommonInclude/pdb/cryptosupport.h \
    ../CommonInclude/pdb/VariantPtr.h \
    waiter.h \
    myeditor.h \
    dlginserttable.h \
    dlginserthtml.h \
    htmlsyntaxhighlighter.h \
    dlgaskencryptmethod.h \
    iconloader.h \
    dlgmanageicons.h \
    iconmanager.h \
    dbicon.h \
    dlgscanimage.h \
    dlgrotateparams.h \
    imagelabel.h \
    dlgsavescan.h \
    searchrequestres.h

FORMS    += mainwindow.ui \
    notificationdialog.ui \
    treerenamecreatedlg.ui \
    attachmentclickationdialog.ui \
    dlgexportparams.ui \
    treestatisticdlg.ui \
    treesearchdlg.ui \
    passworddlg.ui \
    askchangesdetailsdlg.ui \
    dlginserttable.ui \
    dlginserthtml.ui \
    dlgaskencryptmethod.ui \
    dlgmanageicons.ui \
    dlgscanimage.ui \
    dlgrotateparams.ui \
    dlgsavescan.ui

RESOURCES += \
    pdb_res.qrc

unix:!macx:!symbian|win32: LIBS += -L$$PWD/../../ThreadPoolProject/release/ -lAdvThreadPool

INCLUDEPATH += $$PWD/../../ThreadPoolProject/AdvThreadPool
INCLUDEPATH += /opt/Qt54/5.4/Src/qtbase/include
DEPENDPATH += $$PWD/../../ThreadPoolProject/AdvThreadPool

win32: PRE_TARGETDEPS += $$PWD/../../ThreadPoolProject/AdvThreadPool/AdvThreadPool.lib
else:unix:!macx:!symbian: PRE_TARGETDEPS += $$PWD/../../ThreadPoolProject/release/libAdvThreadPool.a
