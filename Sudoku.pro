QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sudoku
TEMPLATE = app

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    sudokualgorithm.cpp \
    sudokuview.cpp \
    sudokuboard.cpp

HEADERS += \
        mainwindow.h \
    sudokualgorithm.h \
    sudokuview.h \
    sudokuboard.h

FORMS += \
        mainwindow.ui
