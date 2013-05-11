#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "boardmodel.h"

class GameModel{
private:
    BoardModel board;
    TetrixPiece currentPiece;
    TetrixPiece nextPiece;
public:
    GameModel(int width, int height):
        board(BoardModel(width, height)){
    }

    const BoardModel &getBoard() const;
    void setBoard(const BoardModel &value);
    const TetrixPiece &getCurrentPiece() const;
    void setCurrentPiece(const TetrixPiece &value);
    const TetrixPiece &getNextPiece() const;
    void setNextPiece(const TetrixPiece &value);
};

#endif // GAMEMODEL_H