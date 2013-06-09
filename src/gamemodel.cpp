#include "gamemodel.h"

const BoardModel &GameModel::getBoard() const
{
    return board;
}

void GameModel::setBoard(const BoardModel &value)
{
    board = value;
}

const Tetronimo &GameModel::getCurrentPiece() const
{
    return currentPiece;
}

void GameModel::setCurrentPiece(const Tetronimo &value)
{
    currentPiece = value;
}

const Tetronimo &GameModel::getNextPiece() const
{
    return nextPiece;
}

void GameModel::setNextPiece(const Tetronimo &value)
{
    nextPiece = value;
}
