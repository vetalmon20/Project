#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ui_mainwindow.h"


class SudokuView;
class SudokuAlgorithm;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
signals:
    void keypress(const QString &);
public slots:
    void handleWin(const int signal);
private:
    Ui::MainWindow *ui;
    SudokuAlgorithm *algorithm;
    SudokuView *view;
    QPoint b_clickpoint;

};

#endif // MAINWINDOW_H
