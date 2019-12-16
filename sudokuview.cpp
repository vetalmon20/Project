#include "sudokuview.h"
#include "sudokuboard.h"

#include <QPainter>
#include <iostream>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>

SudokuView::SudokuView(QWidget *parent) : QWidget(parent)
{
    connect(this, &SudokuView::clicked, this, &SudokuView::select);
    connect(this, &SudokuView::clickedButton, this, &SudokuView::buttonClicked);
    connect(this, &SudokuView::clickedDialogButton, this, &SudokuView::dialogButtonClicked);

}

SudokuView::~SudokuView()
{

}

void SudokuView::setBoard(SudokuBoard *board)
{
    b_board = board;
    //connect
    connect(board, SIGNAL(dataChanged()), this, SLOT(update()));
    connect(this, SIGNAL(newGameStart(int)), b_board, SLOT(startNewGame(int)));
    updateGeometry();

}

SudokuBoard *SudokuView::board() const
{
    return b_board;
}

QSize SudokuView::cellSize() const
{
    return b_cellSize;
}

QSize SudokuView::totalSize() const
{
    QSize boardSize = QSize(cellSize().width() * b_board->columns() +1,
                            cellSize().height() * b_board->rows() +1);
    return boardSize;
}

QRect SudokuView::cellRect(int row, int column) const
{
    QRect cRect = QRect(QPoint(column*cellSize().width(), row*cellSize().height()), cellSize());
    return cRect;

}

QPoint SudokuView::atCell(const QPoint &point) const
{
    if(point.x()<0 || point.y()<0 || point.x()>cellSize().width()*9 ||
       point.y()>cellSize().height()*9) return QPoint(*selectedPoint);
    int column=point.x() / cellSize().width();
    int row=point.y() / cellSize().height();
    return QPoint(row, column);

}

int SudokuView::atButton(const QPoint &pt) const
{
    for(int number=1; number<=3; number++) {
        QRect button = buttonRect(number);
        if(pt.x() > button.topLeft().x() && pt.x() < button.topRight().x() &&
           pt.y() > button.top() && pt.y() < button.bottom()) {
            return number;
        }
    }
    return 0;
}

int SudokuView::atDialogButton(const QPoint &pt) const
{
    for(int number=1; number<=4; number++) {
        QRect button = dialogButtonRect(number);
        if(pt.x() > button.topLeft().x() && pt.x() < button.topRight().x() &&
           pt.y() > button.top() && pt.y() < button.bottom()) {
            return number;
        }
    }
    return 0;
}

QRect SudokuView::buttonRect(int number) const
{
    QRect bRect = QRect(QPoint(totalSize().width() + cellSize().width() * 2, cellSize().height()*2*number),
                        buttonzSize);
    return bRect;
}

QRect SudokuView::dialogButtonRect(int number) const
{
    QRect dbRect = QRect(QPoint(dialogTopLeft.x()+36+90*(number-1), dialogTopLeft.y() + 50),
                         dialogButtonSize);
    return dbRect;
}

void SudokuView::select(const QPoint &cell)
{
    selectedPoint->setX(cell.x());
    selectedPoint->setY(cell.y());
    b_board->setSelectedPoint(cell.x(), cell.y());
    update();
}

void SudokuView::pressed(const QString &input)
{
    if(selectedPoint->x()==-1) return;
    if(input!="1" && input!="2" && input!="3" && input!="4" && input!="5" &&
            input!="6" && input!="7" && input!="8" && input!="9") return;
    int number = input.toInt();

    b_board->changeData(number);
    update();

}

void SudokuView::buttonClicked(const int &number)
{
    if(number==1) {
        newGameButton=true;
        update();
    }
    else if(number==2) {
        selectedPoint->setX(-1);
        selectedPoint->setY(-1);
        b_board->solveIt();
        update();
    }
    else if(number==3) {
        customMode=false;
        b_board->customFinish(false);
        update();
    }
}

void SudokuView::dialogButtonClicked(const int &number)
{
    newGameButton=false;
    if(number == 1) emit newGameStart(1);
    else if(number == 2) emit newGameStart(2);
    else if(number == 3) emit newGameStart(3);
    else if(number == 4) {
        emit newGameStart(4);
        customMode=true;
    }
    setBeginning(true);
    update();
}

void SudokuView::resetBoard()
{

}

void SudokuView::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    //board
    for(int i = 0; i < b_board->columns(); ++i) {
        for(int j = 0; j < b_board->rows(); ++j) {
            painter.save();
            drawCell(&painter, i, j);
            painter.restore();
        }
    }

    //border
    painter.save();
    drawBorder(&painter);
    painter.restore();
    //hightlight

    if(selectedPoint->x() != -1) {
        painter.save();
        drawHighlight(&painter, selectedPoint->x(), selectedPoint->y());
        painter.restore();
    }

    //conflict
    if(b_board->getConflictIndex().size() != 0) {
        painter.save();
        for(int i = 0; i<b_board->getConflictIndex().size(); i++)
            drawConflict(&painter, b_board->getConflictIndex().at(i)/9, b_board->getConflictIndex().at(i)%9);
        painter.restore();
    }

    //number
    for(int i = 0; i < b_board->columns(); ++i) {
        for(int j = 0; j < b_board->rows(); ++j) {
            painter.save();
            drawNumber(&painter, i, j, b_board->boardData());
            painter.restore();
        }
    }

    //button
    painter.save();
    if(!customMode) {
        drawButton(&painter, "New Game", 1);
        if(beginning) drawButton(&painter, "Solve", 2);
    }
    else drawButton(&painter, "Finish", 3);
    painter.restore();

    //dialog
    painter.save();
    if(newGameButton) {
        drawDialog(&painter);
        drawDialogButton(&painter, 1, "Easy");
        drawDialogButton(&painter, 2, "Medium");
        drawDialogButton(&painter, 3, "Hard");
        drawDialogButton(&painter, 4, "Custom");
    }
    painter.restore();

}

void SudokuView::mouseReleaseEvent(QMouseEvent *event)
{

    QPoint point=atCell(event->pos());
    if(newGameButton) {
        int dialogButtonNumber = atDialogButton(event->pos());
        emit clickedDialogButton(dialogButtonNumber);
        update();
    }
    else {
        int buttonNumber = atButton(event->pos());
        if(buttonNumber != 0) emit clickedButton(buttonNumber);
        else emit clicked(point);
    }
}

void SudokuView::keyPressEvent(QKeyEvent *event)
{
    if(selectedPoint->x()==-1) return;
    QString input=event->text();
    if(input!="1" || input!="2" || input!="3" || input!="4" || input!="5" ||
            input!="6" || input!="7" || input!="8"|| input!="9") return;

    int number = input.toInt();
    emit press(number);



}

void SudokuView::drawCell(QPainter *painter, int row, int column)
{
    QRect rect = cellRect(row, column);
    QFont font = painter->font();
    QPen pen(palette().color(QPalette::Dark));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor("Blue")));
    painter->drawRect(rect);

}

void SudokuView::drawBorder(QPainter *painter)
{
    QPen pen(QColor("Black"));
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(QColor("Black"));

    for(int i=0; i<10; i+=3) {
        QRect startRect=cellRect(i, 0);
        QRect endRect=cellRect(i, 9);
        QPoint startPoint(startRect.topLeft());
        QPoint endPoint(endRect.topLeft());
        QLine line(startPoint, endPoint);
        painter->drawLine(line);
    }

    for(int i=0; i<10; i+=3) {
        QRect startRect=cellRect(0, i);
        QRect endRect=cellRect(9, i);
        QPoint startPoint(startRect.topLeft());
        QPoint endPoint(endRect.topLeft());
        QLine line(startPoint, endPoint);
        painter->drawLine(line);
    }

}

void SudokuView::drawNumber(QPainter *painter, int row, int column, QVector<int> allData)
{

    int index=row*b_board->columns()+column;
    int number=allData.at(index);
    if(number==0) return;
    QString data=QString::number(number);
    QRect rect = cellRect(row, column);
    QColor fillColor("Black");
    QFont font = painter->font();
    font.setPixelSize(24);
    font.setBold(1);
    font.setFamily(QString("Helvica"));
    painter->setBrush(fillColor);
    painter->setPen(fillColor);
    painter->setFont(font);
    painter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, data);

}

void SudokuView::drawHighlight(QPainter *painter, int row, int column)
{
    QRect rect = cellRect(row, column);
    QColor fillColor("Grey");
    painter->fillRect(rect, fillColor);
}

void SudokuView::drawConflict(QPainter *painter, int row, int column)
{
    QRect rect = cellRect(row, column);
    QColor fillColor("Pink");
    painter->fillRect(rect, fillColor);
}

void SudokuView::drawButton(QPainter *painter, QString text, int number)
{
    QRect rect = buttonRect(number);
    QFont font = painter->font();
    QBrush brush("Black");
    font.setPixelSize(18);
    font.setBold(1);
    font.setFamily(QString("Helvica"));
    painter->setPen("Black");
    painter->setBrush(brush);
    painter->drawRect(rect);

    brush.setColor("White");
    painter->setFont(font);
    painter->setPen("White");
    painter->setBrush(brush);
    painter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, text);
}

void SudokuView::drawDialog(QPainter *painter)
{
    QRect rect = QRect(dialogTopLeft, dialogSize);
    QBrush brush("Gray");
    painter->setPen("Black");
    painter->setBrush(brush);
    painter->drawRect(rect);


}

void SudokuView::drawDialogButton(QPainter *painter, int number, QString message)
{
    QRect rect = dialogButtonRect(number);
    QBrush brush("Red");
    QFont font = painter->font();
    font.setPixelSize(18);
    font.setBold(1);
    font.setFamily(QString("Helvica"));

    painter->setPen("Black");
    painter->setBrush(brush);
    painter->drawRect(rect);

    brush.setColor("Black");
    painter->setFont(font);
    painter->setBrush(brush);
    painter->drawText(rect, Qt::AlignVCenter|Qt::AlignHCenter, message);

}

