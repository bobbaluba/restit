#ifndef TETRISBOARD_H
#define TETRISBOARD_H

#include "gamemodel.h"
#include "boris.h"
#include "randomagent.h"
#include "greedyagent.h"
#include "stochyagent.h"
#include "zuckermaas.h"

#include <QBasicTimer>
#include <QFrame>
#include <QPointer>
#include <complextetris.h>
#include <array>


class QLabel;

class TetrisBoard : public QFrame
{
    Q_OBJECT

public:
    explicit TetrisBoard(QWidget *parent = 0);

    void setNextPieceLabel(QLabel *label);
    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    enum { BoardWidth = 10, BoardHeight = 20 };

public slots:
    void start();
    void pause(bool checked);
    void setAISpeed(int speed);
    void setAutoPlay(bool value);
    void setInvisiblePlay(bool value);
    void setParameters(QString newParameters);
    void setNoAI(bool enabled);
    void setGreedyAI(bool enabled);
    void setZuckerAI(bool enabled);
    void setLineDownTimeoutEnabled(bool enabled);
    void setLookAheadEnabled(bool enabled);

signals:
    void levelChanged(int level);
    void linesRemovedChanged(int numLines);
    void maxLinesRemovedChanged(int numGames);
    void avgLinesRemovedChanged(double lines);
    void movingAvgLinesChanged(double lines);
    void gamesPlayedChanged(int numGames);
    void parametersChanged(QString newParameters);
    void totalMovesChanged(int); //has to be int to connect with qlcds

protected:
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void timerEvent(QTimerEvent *event);

private:
    int squareWidth() { return contentsRect().width() / BoardWidth; }
    int squareHeight() { return contentsRect().height() / BoardHeight; }
    void clearBoard();
    void dropDown();
    void oneLineDown();
    void pieceDropped(int dropHeight);
    void newPiece();
    void showNextPiece();
    void refreshGUI();
    void drawSquare(QPainter &painter, int x, int y, TetronimoShape shape);
    void onGameOver();

    QBasicTimer timer, borisTimer;
    QPointer<QLabel> nextPieceLabel;
    //int score;

    bool invisible;
    bool lineDownTimeoutEnabled;
    int gamesPlayed;
    int maxLinesRemoved;
    int totalLinesRemoved;
    enum { NumGamesMovingAverage = 10 };
    std::array<int, NumGamesMovingAverage> lastLinesRemoved;

    GameModel gameModel; //model
    ComplexTetris tetris; //controller
    bool gameOver;

    //ai
    //GreedyBoss greedyBoss; //replaced by stochy
    RandomAgent randomAgent;
    StochyAgent stochyAgent;
    ZuckerMaas zuckerAgent;
    Boris boris;
    bool borisIsPlaying;
    int borisInterval;
    bool autoPlay;
};

#endif
