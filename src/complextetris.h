#ifndef COMPLEXTETRIS_H
#define COMPLEXTETRIS_H

#include <gamemodel.h>
#include <state.h>

class ComplexTetris{
private:
    GameModel* game;
    int numLinesRemoved; //maybe put this into gamemodel
    int curX;
    int curY;
    bool started;
    bool paused;
    bool gameOver;
public:
    explicit ComplexTetris(GameModel* game);
    bool rotateCCW();
    void moveDown();
    bool moveLeft();
    bool moveRight();
    void drop();
    void startNewGame();
    void setPaused(bool paused);
    void timeoutElapsed();

    //getters
    const State getState(){return State(game->getBoard(), game->getCurrentPiece(), game->getNextPiece());}
    bool isPaused() const { return paused; }
    bool isGameOver() const { return gameOver; }
    bool hasStarted() const { return started; }
    int getLinesRemoved() const { return numLinesRemoved; }
    int getLevel() const { return 1 + numLinesRemoved / 10; }
    int getTimeoutTime() const { return 1000 / getLevel(); }
    int getCurrentPieceX() const { return curX; }
    int getCurrentPieceY() const { return curY; }
private:
    //helper/convenience methods
    bool oneLineDown(); //returns false if the piece hit something and was "dropped"
    void droppedPiece();
    bool tryMove(const TetrixPiece &newPiece, int newX, int newY);
    void newPiece();
    const TetrixPiece& curPiece() const { return game->getCurrentPiece(); }
    const TetrixPiece& nextPiece() const { return game->getNextPiece(); }
    const BoardModel& board() const { return game->getBoard(); }
    int getStartColumn() { return game->getWidth() / 2; }
};

#endif // COMPLEXTETRIS_H
