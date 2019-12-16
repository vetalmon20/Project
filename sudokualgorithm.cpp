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

int SudokuAlgorithm::randomChoose(QVector<int> &avNum) {
    int size = static_cast<int>(avNum.size());
    std::uniform_int_distribution<unsigned int> randomInt(0, size-1);

    return randomInt(generator);
}

void SudokuAlgorithm::checkChange()
{
    checkConflict();
    if(!customMode)
        checkWin();
}

void SudokuAlgorithm::resetGame(const int state)
{
    setupBoard(state);
}

void SudokuAlgorithm::customCheck()
{
    customMode=false;
    if(solvable()) {
        qDebug()<<"Good";
    }
    else {
        emit winler(3);
    }
}

void SudokuAlgorithm::solveTheMap()
{
    emit winler(2);
}

void SudokuAlgorithm::checkWin()
{
    for(int i = 0; i < b_board->getMap().size(); i++) {
        if(b_board->getMap().at(i) != b_board->boardData().at(i))
            return;
    }
    emit winler(1);
}

void SudokuAlgorithm::checkConflict()
{

    int row = board()->getSelectedPoint()->x();
    int column = board()->getSelectedPoint()->y();
    if(row == -1 || column == -1) return;
    int data = board()->data(row, column);

    QVector<int>conflictIndex;
    const QVector<int>boardData =board()->boardData();

    //check rows;
    for(int i=column; i<81; i+=9) {
        if(boardData[i] == data && i != (row*9+column))
            conflictIndex.push_back(i);
    }

    //checkcolumn
    for(int j=0+row*9; j<(row*9 + 9); j++) {
        if(boardData[j] == data && j != (row*9+column))
            conflictIndex.push_back(j);
    }

    //checkmatrix
    for(int i=(row/3)*3; i<(row/3)*3+3; i++) {
        for(int j=(column/3)*3; j<(column/3)*3+3; j++) {
            int total=i*9+j;
            if(boardData[total] == data && total != (row*9+column) && conflictIndex.indexOf(data) != -1)
                conflictIndex.push_back(total);
        }
    }
    board()->setConflictIndex(conflictIndex);
}

void SudokuAlgorithm::customInput()
{
    customMode=true;
    b_board->setCustomMode(true);
}

bool SudokuAlgorithm::solvable()
{
    QVector<int> map(b_board->boardData());
    if(!b_board->getConflictIndex().empty()) {
        return false;
    }
    for(int i=0; i<b_board->sudokusize(); i++) {
        if(b_board->boardData()[i] == 0) b_board->setQuestionIndex(i);
    }
    if(b_board->getQuestionIndex().size() == 0) return false;
    if(solve(0, map)) return true;
    else return false;
}

bool SudokuAlgorithm::solve(int index, QVector<int> map)
{
    if(index == b_board->getQuestionIndex().size()) {
        for(int i=0; i<b_board->sudokusize(); i++)
            b_board->setMapData(map[i]);
        return true;
    }
    int questionindex= b_board->getQuestionIndex()[index];
    int row=questionindex/9, column=questionindex%9;
    QVector<int> cando(checkCell(row, column, map));

    while(!cando.empty()) {
        int num = randomChoose(cando);
        map[questionindex] = cando.at(num);
        if(solve(index+1, map)) return true;
        else {
            cando.erase(cando.begin()+num);
        }
    }
    return false;
}


QVector<int> SudokuAlgorithm::checkCell(int row, int column, QVector<int> board)
{
    QVector<bool>canDo;
    canDo.fill(false, 10);

    //check rows;
    for(int i=column; i<81; i+=9) {
        if(board[i] != 0)
            canDo[board[i]] = true;
    }

    //checkcolumn
    for(int j=0+row*9; j<( row*9 + 9); j++) {
        if(board[j] != 0)
            canDo[board[j]] = true;
    }

    //checkmatrix
    for(int i=(row/3)*3; i<(row/3)*3+3; i++) {
        for(int j=(column/3)*3; j<(column/3)*3+3; j++) {
            int total=i*9+j;
            if(board[total] != 0)
                canDo[board[total]] = true;
        }
    }

    QVector<int>canDoList;
    for(int i=1; i<10; i++)
        if(canDo[i] == false)
            canDoList.push_back(i);

    return canDoList;
}

void SudokuAlgorithm::pressed(const QString &input)
{
    if(b_board->getSelectedPoint()->x()==-1) return;
    if(input!="1" && input!="2" && input!="3" && input!="4" && input!="5" &&
            input!="6" && input!="7" && input!="8" && input!="9") return;
    int number = input.toInt();
    b_board->changeData(number);
}



void SudokuAlgorithm::setupBoard(int level)
{
    if(b_board != nullptr) delete b_board;
    b_board = new SudokuBoard();
    emit setNewBoard();
    connect(b_board, SIGNAL(dataChanged()), this, SLOT(checkChange()));
    connect(b_board, SIGNAL(boardReset(int)), this, SLOT(resetGame(int)));
    connect(b_board, SIGNAL(checkCustom()), this, SLOT(customCheck()));
    connect(b_board, SIGNAL(solveTheMap()), this, SLOT(solveTheMap()));
    generator.seed(gameTimes++);
    if(level!=0) {
        switch (level) {
        case 1:
            createQuestion(30);
            break;
        case 2:
            createQuestion(45);
            break;
        case 3:
            createQuestion(60);
        case 4:
            customInput();
            break;
        }
    }
    else createQuestion();
}

