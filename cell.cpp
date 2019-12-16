#include <QDebug>
#include <QRegExpValidator>
#include "cell.h"
#include "board.h"

#define CELL_SIZE 25

Cell::Cell(Board * board, int row, int col, int value) :
    QLineEdit(dynamic_cast<QWidget *>(board)),
    board(board), row(row), col(col), value(value)
{
    this->setFixedSize(CELL_SIZE,CELL_SIZE);
    this->setAlignment(Qt::AlignCenter);
    if (value) {
        this->setText(QString::number(value));
        this->setDisabled(true);
    } else {
        QRegExp regex("[1-9]{1}");
        QValidator * validator = new QRegExpValidator(regex, this);
        this->setValidator(validator);
        connect(this, SIGNAL(textChanged(QString)), this, SLOT(valueEntered()));
    }
}

void Cell::valueEntered()
{
    QPalette * palette = new QPalette();
    palette->setColor(QPalette::Text,Qt::black);
    this->setPalette(* palette);

    if (!board->setValue(this->row, this->col, this->text().toInt())) {
        QPalette * palette = new QPalette();
        palette->setColor(QPalette::Text,Qt::red);
        this->setPalette(* palette);
    }
}
