#include "sudokualgorithm.h"
#include "sudokuboard.h"
#include <random>
#include <QDebug>
#include <iostream>

using namespace std;

SudokuAlgorithm::SudokuAlgorithm(QObject *parent) : QObject(parent)
{
}

void SudokuAlgorithm::newGame()
{
    setupBoard();
}


SudokuBoard *SudokuAlgorithm::board() const
{
    return b_board;
}

void SudokuAlgorithm::createMap()
{

    QVector<int>map;
    map.fill(0, b_board->sudokusize());
    int index=0;
    keepMoving(index, map);

}

void SudokuAlgorithm::createQuestion(int numbersOfEmpty)
{
    if(numbersOfEmpty == -1) return;
    createMap();
    for(int i=0; i<numbersOfEmpty; i++) {
        std::uniform_int_distribution<unsigned int> randomInt(0, b_board->sudokusize()-1);
        b_board->setQuestionIndex(randomInt(generator));

    }

    // set map
    for(int i=0; i<b_board->sudokusize(); i++) {
        b_board->setData(i/9, i%9, b_board->getMap()[i]);
    }
    for(int i=0; i<b_board->getQuestionIndex().size(); i++) {
        int index=b_board->getQuestionIndex().at(i);
        b_board->setData(index/9, index%9, 0);
    }
}

bool SudokuAlgorithm::keepMoving(int index, QVector<int> map)
{
    if(index == b_board->sudokusize()) {
        for(int i=0; i<81; i++) {
            this->b_board->setMapData(map[i]);
        }
        return true;
    }
    int row=index/9, column=index%9;
    QVector<int> cando(checkCell(row, column, map));

    while(!cando.empty()) {
        int num = randomChoose(cando);
        map[index] = cando.at(num);
        if(keepMoving(index+1, map)) return true;
        else {
            cando.erase(cando.begin()+num);
        }

    }
    return false;
}

