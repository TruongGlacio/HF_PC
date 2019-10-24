
QT+= core gui network widgets multimedia
TARGET = HF_Gui

TEMPLATE = app

CONFIG += c++11 staticlib*

SOURCES += main.cpp\
        mainwindow.cpp \

HEADERS += mainwindow.h \

FORMS += mainwindow.ui \

DISTFILES += \
    sound_off.png \
    sound_on.png \
    HF_Gui.desktop

RESOURCES +=  image.qrc\

#QMAKE_LFLAGS+= -static-libgcc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
