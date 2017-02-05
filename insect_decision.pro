TEMPLATE = app
TARGET = insect_decision

QT = core gui
QT += charts
QT += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    models.cpp \
    chart_um.cpp \
    chart_pratt.cpp \
    chart_indirect_britton.cpp \
    chart_direct_britton.cpp \
    chart_gaze.cpp

HEADERS += \
    mainwindow.h \
    models.h \
    chart_um.h \
    chart_pratt.h \
    chart_indirect_britton.h \
    chart_direct_britton.h \
    chart_gaze.h
