#include "mainwindow.h"
#include "sudokualgorithm.h"
#include "sudokuview.h"
#include <QLayout>
#include <QKeyEvent>
#include <QMessageBox>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    view = new SudokuView();
    algorithm = new SudokuAlgorithm(this);
    connect(this, &MainWindow::keypress, algorithm, &SudokuAlgorithm::pressed);
    connect(algorithm, &SudokuAlgorithm::winler, this, &MainWindow::handleWin);
    connect(algorithm, &SudokuAlgorithm::setNewBoard, this, &MainWindow::connectNewBoard);
    algorithm->newGame();
    setCentralWidget(view);
    view->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleWin(const int signal)
{
    view->setBeginning(false);
    if(signal == 1) {
        QMessageBox::information(this, "Good", "yooo!");
    }
    else if(signal == 2) {
        QMessageBox::information(this, "Bad", "Solved!");
    }

    else if(signal == 3) {
        QMessageBox::information(this, "Bad", "Invalid Input!");
    }
}

void MainWindow::connectNewBoard()
{
    view->setBoard(algorithm->board());
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QString input = event->text();
    emit keypress(input);
}

