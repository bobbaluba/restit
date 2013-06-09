#include "complextetris.h"

ComplexTetris::ComplexTetris(GameModel *game):
    game(game),
    numLinesRemoved(0),
    started(false),
    paused(false),
    gameOver(false)
{
    Tetronimo piece = Tetronimo::getRandomPiece();
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
    gameOver = false;
    numLinesRemoved = 0;
    BoardModel emptyBoard(game->getWidth(), game->getHeight());
    game->setBoard(emptyBoard);
    game->setNextPiece(Tetronimo::getRandomPiece());
    newPiece();
}

void ComplexTetris::setPaused(bool paused){
    if (!hasStarted()){
        return;
    }
    this->paused = paused;
}

void ComplexTetris::timeoutElapsed(){
    if(hasStarted() && !isPaused() && !isGameOver()){
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

bool ComplexTetris::tryMove(const Tetronimo &newPiece, int newX, int newY){
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
    game->setNextPiece(Tetronimo::getRandomPiece());
    curX = getStartColumn();
    curY = game->getHeight() - 1 + curPiece().minY();

    if (!tryMove(curPiece(), curX, curY)) {
        //game over
        gameOver = true;
    }
}
