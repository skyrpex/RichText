TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG -= console
CONFIG += C++11

INCLUDEPATH += SFML

LIBS += -lsfml-system-d -lsfml-window-d -lsfml-graphics-d -ljpeg -lGLEW

SOURCES += main.cpp \
    ../RichText.cpp

HEADERS += \
    ../RichText.hpp
