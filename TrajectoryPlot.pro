QT += core gui
QT += core gui network
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

TARGET = TrajectoryPlot
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    configreader.cpp \
    main.cpp \
    mainwindow.cpp \
    tcpserver.cpp \
    udpconnection.cpp \
    qcustomplot.cpp


HEADERS += \
    configreader.h \
    mainwindow.h \
    tcpserver.h \
    trajectorypoint.h \
    udpconnection.h \
    qcustomplot.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    config.ini
