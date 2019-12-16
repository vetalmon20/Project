#include <QDebug>
#include <stdlib.h>
#include "board.h"
#include "cell.h"

Board::Board(QWidget* parent) : QWidget(parent)
{
    init();
}

void Board::init()
{
    // The value grid will hold only values
    for (int row = 0; row < AXES_SIZE; row++) {
        for (int col = 0; col < AXES_SIZE; col++) {
            valueGrid[row][col] = 0;
        }
    }

    // Read start values from file and put in valueGrid
    char buf[6];
    int row;
    int col;
    int value;
    FILE * file = fopen("start.txt", "r");
    while (fgets(buf, 6, file)) {
        if (buf[0] != '\n') {
            row = atoi(&buf[0]);
            col = atoi(&buf[2]);
            value = atoi(&buf[4]);
            valueGrid[row][col] = value;
        }
    }
    fclose(file);

    // The cell grid will hold the cells
    cellGrid = new QGridLayout();

    // Initialize cells
    for (int row = 0; row < AXES_SIZE; row++) {
        for (int col = 0; col < AXES_SIZE; col++) {
            int value = valueGrid[row][col];
            Cell * cell = new Cell(this, row, col, value);

            if ((row == 2 || row == 5) && (col == 2 || col == 5)) {
                cell->setStyleSheet("margin-right: 10px; margin-bottom: 10px");
                cell->setFixedSize(35, 35);
            } else if (row == 2 || row == 5) {
                cell->setStyleSheet("margin-bottom: 10px");
                cell->setFixedHeight(35);
            } else if (col == 2 || col == 5) {
                cell->setStyleSheet("margin-right: 10px");
                cell->setFixedWidth(35);
            }
            cellGrid->addWidget(cell, row, col);
        }
    }
    this->setLayout(cellGrid);
}

bool Board::setValue(int row, int col, int value) {
    if (check(row,col,value)) {
        valueGrid[row][col] = value;
        return true;
    }
    return false;
}

bool Board::check(int row, int col, int value) {
    return checkRow(row, value) && checkCol(col, value) && checkBlock(row, col, value);
}

bool Board::checkRow(int row, int value) {
    for (int i = 0; i < AXES_SIZE; i++) {
        if (valueGrid[row][i] == value) {
            return false;
        }
    }
    return true;
}

bool Board::checkCol(int col, int value) {
    for (int i = 0; i < AXES_SIZE; i++) {
        if (valueGrid[i][col] == value) {
            return false;
        }
    }
    return true;
}

bool Board::checkBlock(int row, int col, int value) {
    int startRow = getStartRow(row);
    int startCol = getStartCol(col);
    for (int i = startRow; i < startRow+3; i++) {
        for (int j = startCol; j < startCol+3; j++) {
            if (valueGrid[i][j] == value) {
                return false;
            }
        }
    }
    return true;
}

int Board::getStartRow(int row) {
    if (row <= 2) {
        return 0;
    } else if (row <= 5) {
        return 3;
    }
    return 6;
}

int Board::getStartCol(int col) {
    if (col <= 2) {
        return 0;
    } else if (col <= 5) {
        return 3;
    }
    return 6;
}
