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



protected:
    bool keepMoving(int index, QVector<int> map);

private:
    SudokuBoard* b_board=nullptr;
    std::default_random_engine generator;
    int gameTimes = 1;
    bool customMode=false;
};

#endif // SUDOKUALGORITHM_H
