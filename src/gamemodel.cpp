#include "gamemodel.h"

BoardModel GameModel::getBoard() const
{
    return board;
}

void GameModel::setBoard(const BoardModel &value)
{
    board = value;
}

TetrixPiece GameModel::getCurrentPiece() const
{
    return currentPiece;
}

void GameModel::setCurrentPiece(const TetrixPiece &value)
{
    currentPiece = value;
}

TetrixPiece GameModel::getNextPiece() const
{
    return nextPiece;
}

void GameModel::setNextPiece(const TetrixPiece &value)
{
    nextPiece = value;
}
