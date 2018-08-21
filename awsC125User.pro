#-------------------------------------------------
#
# Project created by QtCreator 2017-07-25T15:58:34
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = awsC125User
TEMPLATE = app

DESTDIR = $$_PRO_FILE_PWD_/../

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += ../../aws125CommonInclude

#####   for IRON BUTTONS & HANDWHEELS   ###########
include($$_PRO_FILE_PWD_/../../aws125Qmake/check_mdis.pri)
message( USE_MDIS $$USE_MDIS )
include($$_PRO_FILE_PWD_/../../Klm/klm.pri)
###################################################

SOURCES += main.cpp\
        drawIKO.cpp \
        parsers.cpp \
        multiLangSupportC.cpp \
        qlabelm.cpp \
    ../../aws125CommonSrc/sharedMemoryOptions.cpp \
    ../../aws125CommonSrc/multiLanguageSupport.cpp \
    ../../aws125CommonSrc/crc16Utils.cpp \
    ../../aws125CommonSrc/diagnParserUtils.cpp \
    ../../aws125CommonSrc/commonUtils.cpp \
    ../../aws125CommonSrc/drawScales.cpp \
    mouseOptions.cpp \
    drawp18.cpp \
    mainWindowC.cpp \
    drawEchoIKO.cpp

HEADERS  += \
        settingsC.h \        
        qlabelm.h \
        scaleInfo.h \
    mainWindowC.h

FORMS    += mainwindow.ui

RESOURCES += \
    resgui.qrc
