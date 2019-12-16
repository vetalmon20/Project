#ifndef CELL_H
#define CELL_H

#include <QLineEdit>
#include "board.h"

class Cell : public QLineEdit
{
    Q_OBJECT
public:
    explicit Cell(Board *, int, int, int);
public slots:
    void valueEntered();
private:
    Board * board;
    int row;
    int col;
    int value;
};

#endif // CELL_H
