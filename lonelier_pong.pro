QT += core gui widgets

TARGET = lonelier_pong
TEMPLATE = app
CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    game_widget.cpp

HEADERS += \
    game_widget.h
