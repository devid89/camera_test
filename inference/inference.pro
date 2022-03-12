TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
QT += core gui


SOURCES += \
        main.cpp

# Qt 4 & 5 specific
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

