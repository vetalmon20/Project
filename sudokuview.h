#ifndef SUDOKUVIEW_H
#define SUDOKUVIEW_H

#include <QWidget>

class SudokuBoard;
class SudokuView : public QWidget
{
    Q_OBJECT
public:
    explicit SudokuView(QWidget *parent = nullptr);
    ~SudokuView();
    void setBoard(SudokuBoard *board);
    SudokuBoard* board() const;
    QSize cellSize() const;
    QSize totalSize() const;
    QRect cellRect(int row, int column) const;
    QPoint atCell(const QPoint &pt) const;
    int atButton(const QPoint &pt) const;
    int atDialogButton(const QPoint &pt) const;
    QRect buttonRect(int number) const;
    QRect dialogButtonRect(int number) const;
    void setBeginning(bool flag){beginning=flag;}

signals:
    void clicked(const QPoint &);
    void press(const int &);
    void clickedButton(const int &);
    void clickedDialogButton(const int &);
    void newGameStart(const int level);

public slots:
    void select(const QPoint &cell);
    void pressed(const QString &input);
    void buttonClicked(const int &number);
    void dialogButtonClicked(const int &number);
    void resetBoard();

protected:
    void paintEvent(QPaintEvent *);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void drawCell(QPainter *painter, int row, int column);
    void drawBorder(QPainter *painter);
    void drawNumber(QPainter *painter, int row, int column, QVector<int> allData);
    void drawHighlight(QPainter *painter, int row, int column);
    void drawConflict(QPainter *painter, int row, int column);
    void drawButton(QPainter *painter, QString text, int number);
    void drawDialog(QPainter *painter);
    void drawDialogButton(QPainter *painter, int number, QString message);

private:
    SudokuBoard *b_board;
    const QSize b_cellSize = QSize(50, 50);
    const QSize buttonzSize = QSize(120, 50);
    const QSize dialogSize = QSize(400, 150);
    const QSize dialogButtonSize = QSize(74, 50);
    const QPoint dialogTopLeft= QPoint(125, 125);
    QPoint *selectedPoint = new QPoint(-1, -1);
    bool newGameButton=false;
    bool customMode=false;
    bool beginning=false;
};

#endif // SUDOKUVIEW_H
