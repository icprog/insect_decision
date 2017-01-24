TEMPLATE = app
TARGET = usher_mclelland

QT = core gui
QT += charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    models.cpp \
    chart_um.cpp \
    chart_pratt.cpp

HEADERS += \
    mainwindow.h \
    models.h \
    chart_um.h \
    chart_pratt.h
