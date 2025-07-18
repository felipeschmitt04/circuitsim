QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

ICON = images/logo.png

SOURCES += \
    ajudadialog.cpp \
    astar.cpp \
    canvaswidget.cpp \
    circuito.cpp \
    componente.cpp \
    conexao.cpp \
    main.cpp \
    mainwindow.cpp \
    sobredialog.cpp

HEADERS += \
    ajudadialog.h \
    astar.h \
    canvaswidget.h \
    circuito.h \
    componente.h \
    conexao.h \
    mainwindow.h \
    sobredialog.h

FORMS += \
    ajudadialog.ui \
    mainwindow.ui \
    sobredialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
