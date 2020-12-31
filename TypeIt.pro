TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
#CONFIG -= qt

SOURCES += main.cpp \
    Book.cpp \
    Scene.cpp

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    Book.h \
    Scene.h
