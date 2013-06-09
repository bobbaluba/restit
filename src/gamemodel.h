#ifndef GAMEMODEL_H
#define GAMEMODEL_H

#include "boardmodel.h"

class GameModel{
private:
    BoardModel board;
    Tetronimo currentPiece;
    Tetronimo nextPiece;
public:
    explicit GameModel(int width, int height):
        board(BoardModel(width, height)){
    }
    explicit GameModel(const BoardModel& board, const Tetronimo& currentPiece, const Tetronimo& nextPiece) :
        board(board),
        currentPiece(currentPiece),
        nextPiece(nextPiece)
    {}

    const BoardModel &getBoard() const;
    void setBoard(const BoardModel &value);
    const Tetronimo &getCurrentPiece() const;
    void setCurrentPiece(const Tetronimo &value);
    const Tetronimo &getNextPiece() const;
    void setNextPiece(const Tetronimo &value);

    //convenience methods
    int getWidth() {return getBoard().getWidth(); }
    int getHeight() {return getBoard().getHeight(); }
};

#endif // GAMEMODEL_H
