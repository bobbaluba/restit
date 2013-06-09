#ifndef TETRISWINDOW_H
#define TETRISWINDOW_H

#include <QFrame>
#include <QWidget>
#include <QRadioButton>
#include <QGroupBox>


class QLCDNumber;
class QLabel;
class QPushButton;

class TetrisBoard;

class TetrisWindow : public QWidget
{
    Q_OBJECT

public:
    explicit TetrisWindow();

private:
    QLabel *createLabel(const QString &text);
    QGroupBox *createAISelector();

    TetrisBoard *board;
    QLabel *nextPieceLabel;
    QLCDNumber *scoreLcd;
    QLCDNumber *levelLcd;
    QLCDNumber *linesLcd;
    QPushButton *startButton;
    QPushButton *quitButton;
    QPushButton *pauseButton;

    QRadioButton *radioButton;
};

#endif
