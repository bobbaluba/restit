#include "gamemodel.h"

const BoardModel &GameModel::getBoard() const
{
    return board;
}

void GameModel::setBoard(const BoardModel &value)
{
    board = value;
}

const TetrixPiece &GameModel::getCurrentPiece() const
{
    return currentPiece;
}

void GameModel::setCurrentPiece(const TetrixPiece &value)
{
    currentPiece = value;
}

const TetrixPiece &GameModel::getNextPiece() const
{
    return nextPiece;
}

void GameModel::setNextPiece(const TetrixPiece &value)
{
    nextPiece = value;
}
