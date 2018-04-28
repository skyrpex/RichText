TEMPLATE = app
CONFIG -= qt
CONFIG -= app_bundle
CONFIG -= console
CONFIG += C++11

INCLUDEPATH += SFML

LIBS += -lsfml-graphics-d -lsfml-window-d -lsfml-graphics-d

SOURCES += main.cpp \
    ../RichText.cpp

HEADERS += \
    ../RichText.hpp
