#ifndef SUDOKUBOARD_H
#define SUDOKUBOARD_H

#include <QObject>
#include <QVector>
#include <QPoint>

class SudokuBoard : public QObject
{
    Q_OBJECT
public:
    explicit SudokuBoard(QObject *parent = nullptr);
    int rows() const { return b_row; }
    int columns() const { return b_column; }
    int data(int row, int column) const;
    int sudokusize() const {return sudokuSize;}
    void setData(int row, int column, int value);
    QVector<int> getQuestionIndex() const{return questionIndex;}
    void setQuestionIndex(int index);
    void changeData(int num);
    void setSelectedPoint(int x, int y);
    QPoint *getSelectedPoint() const{return selectedPoint;}
    QVector<int> getConflictIndex() const {return conflictIndex;}

signals:
    void dataChanged();
    void boardReset(const int state);
    void checkCustom();
    void solveTheMap();
public slots:
    void startNewGame(const int level);
private:
    const int b_row=9, b_column=9, sudokuSize=81;
    QVector<int> b_data;
    QVector<int> solution;
    QVector<int> conflictIndex;
    QPoint *selectedPoint = new QPoint(-1, -1);
    bool customMode=false;


};

#endif // SUDOKUBOARD_H
