#-------------------------------------------------
#
# Project created by QtCreator 2015-06-21T17:33:32
#   (and by Jeriek Van den Abeele)
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia

TARGET = PacmanGame
TEMPLATE = app

CONFIG += C++11

SOURCES += main.cpp\
    gamewindow.cpp \
    startwindow.cpp \
    gameobject.cpp \
    gamecharacter.cpp \
    pellet.cpp \
    gameboard.cpp \
    powerball.cpp \
    pacman.cpp \
    ghost.cpp \
    gametext.cpp

HEADERS  += \
    gamewindow.h \
    startwindow.h \
    gameobject.h \
    gamecharacter.h \
    pellet.h \
    gameboard.h \
    powerball.h \
    pacman.h \
    ghost.h \
    gametext.h

FORMS    += \
    gamewindow.ui \
    startwindow.ui

RESOURCES += \
    images.qrc \
    sounds.qrc
