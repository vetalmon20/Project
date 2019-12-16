#ifndef SUDOKUALGORITHM_H
#define SUDOKUALGORITHM_H

#include <QObject>
#include <random>
class SudokuBoard;
class SudokuAlgorithm : public QObject
{
    Q_OBJECT
public:
    explicit SudokuAlgorithm(QObject *parent = nullptr);
    void newGame();
    SudokuBoard* board() const;
    void createMap();
    void createQuestion(int numbersOfEmpty=-1);
    QVector<int> checkCell(int row, int column, QVector<int>);

signals:
    void winler(const int signal);
    void setNewBoard();

public slots:
    void pressed(const QString &input);
    void checkChange();
    void resetGame(const int state);
    void customCheck();
    void solveTheMap();


protected:
    void setupBoard(int level=0);
    bool keepMoving(int index, QVector<int> map);
    int randomChoose(QVector<int> &avNum);
    void checkWin();
    void checkConflict();
    void customInput();
    bool solvable();
    bool solve(int index, QVector<int> map);

private:
    SudokuBoard* b_board=nullptr;
    std::default_random_engine generator;
    int gameTimes = 1;
    bool customMode=false;
};

#endif // SUDOKUALGORITHM_H
