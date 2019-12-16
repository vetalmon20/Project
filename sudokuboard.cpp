#include "sudokuboard.h"
#include "sudokualgorithm.h"
#include <QDebug>


SudokuBoard::SudokuBoard(QObject *parent) : QObject(parent)
{
    initBoard();

}

int SudokuBoard::data(int row, int column) const
{
    int index=row*rows() + column;
    return b_data[index];
}


void SudokuBoard::setData(int row, int column, int value)
{
    int index=row*9 + column;
    b_data[index] = value;
}

QVector<int> SudokuBoard::boardData() const
{
    QVector<int> pass(b_data);
    return pass;
}

QVector<int> SudokuBoard::getMap() const
{
    return map;
}

void SudokuBoard::setMapData(int value)
{
    map.push_back(value);
}

void SudokuBoard::setQuestionIndex(int index)
{
    questionIndex.push_back(index);
}
void SudokuBoard::setSelectedPoint(int x, int y)
{
    selectedPoint->setX(x);
    selectedPoint->setY(y);
}

void SudokuBoard::setConflictIndex(QVector<int> &index)
{
    QVector<int>newOne(index);
    conflictIndex = newOne;
}

void SudokuBoard::customFinish(bool flag)
{
    setCustomMode(flag);
    emit checkCustom();
}

void SudokuBoard::setInternalData(int row, int column, int value)
{
    int index=row*9 + column;
    b_data[index] = value;
    emit dataChanged();
}

void SudokuBoard::setCustomData(int row, int column, int value)
{
    int index=row*9 + column;
    b_data[index] = value;
    emit dataChanged();
}
rdReset(level);
}
