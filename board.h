#ifndef BOARD_H
#define BOARD_H

#include <QWidget>
#include <QGridLayout>

#define AXES_SIZE 9

class Board : public QWidget
{
    Q_OBJECT
public:
    explicit Board(QWidget * parent = nullptr);
    void init();
    bool setValue(int, int, int);
private:
    bool check(int, int, int);
    bool checkRow(int, int);
    bool checkCol(int, int);
    bool checkBlock(int,int,int);
    int getStartRow(int);
    int getStartCol(int);
    QGridLayout * cellGrid;
    int valueGrid[AXES_SIZE][AXES_SIZE];
};

#endif // BOARD_H
