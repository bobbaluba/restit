#include "complextetris.h"

ComplexTetris::ComplexTetris(GameModel *game):
    game(game),
    numLinesRemoved(0),
    started(false),
    paused(false)
{
    TetrixPiece piece = TetrixPiece::getRandomPiece();
    game->setNextPiece(piece);
}

bool ComplexTetris::rotateCCW(){
    return tryMove(curPiece().rotatedLeft(), curX, curY);
}

void ComplexTetris::moveDown(){
    oneLineDown();
}

bool ComplexTetris::moveLeft(){
    return tryMove(curPiece(), curX - 1, curY);
}

bool ComplexTetris::moveRight(){
    return tryMove(curPiece(), curX + 1, curY);
}

void ComplexTetris::drop(){
    int newY = curY;
    while (newY > 0) {
        if (!tryMove(curPiece(), curX, newY - 1))
            break;
        --newY;
    }
    droppedPiece();
}

void ComplexTetris::startNewGame(){
    paused = false;
    started = true;
    numLinesRemoved = 0;
    BoardModel emptyBoard(game->getWidth(), game->getHeight());
    game->setBoard(emptyBoard);
    game->setNextPiece(TetrixPiece::getRandomPiece());
    newPiece();
}

void ComplexTetris::togglePaused(){
    if (!isStarted()){
        return;
    }
    paused = !isPaused();
}

void ComplexTetris::timeoutElapsed(){
    if(isStarted() && !isPaused()){
        oneLineDown();
    }
}

bool ComplexTetris::oneLineDown(){
    bool moved = tryMove(curPiece(), curX, curY - 1);
    if (!moved){
        droppedPiece();
    }
    return moved;
}

void ComplexTetris::droppedPiece(){
    int numFullLines;
    BoardModel newBoard = game->getBoard().placePiece(curPiece(), curX, curY, &numFullLines);
    numLinesRemoved += numFullLines;
    game->setBoard(newBoard);
    newPiece();
}

bool ComplexTetris::tryMove(const TetrixPiece &newPiece, int newX, int newY){
    if(board().isFree(newPiece, newX, newY)){
        game->setCurrentPiece(newPiece);
        curX = newX;
        curY = newY;
        return true;
    } else {
        return false;
    }
}

void ComplexTetris::newPiece(){
    game->setCurrentPiece(game->getNextPiece());
    game->setNextPiece(TetrixPiece::getRandomPiece());
    curX = getStartColumn();
    curY = game->getHeight() - 1 + curPiece().minY();

    if (!tryMove(curPiece(), curX, curY)) {
        //game over
        started = false;
    }
}