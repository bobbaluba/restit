#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "boardmodel.h"

class GameModel{
private:
    BoardModel board;
    TetrixPiece currentPiece;
    TetrixPiece nextPiece;
public:
    explicit GameModel(int width, int height):
        board(BoardModel(width, height)){
    }
    explicit GameModel(const BoardModel& board, const TetrixPiece& currentPiece, const TetrixPiece& nextPiece) :
        board(board),
        currentPiece(currentPiece),
        nextPiece(nextPiece)
    {}

    const BoardModel &getBoard() const;
    void setBoard(const BoardModel &value);
    const TetrixPiece &getCurrentPiece() const;
    void setCurrentPiece(const TetrixPiece &value);
    const TetrixPiece &getNextPiece() const;
    void setNextPiece(const TetrixPiece &value);

    //convenience methods
    int getWidth() {return getBoard().getWidth(); }
    int getHeight() {return getBoard().getHeight(); }
};

#endif // GAMEMODEL_H
