/****************************************************************************
 **
 ** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
 ** Contact: http://www.qt-project.org/legal
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
 **     of its contributors may be used to endorse or promote products derived
 **     from this software without specific prior written permission.
 **
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

#include <QtGui>

#include <cassert>

#include "tetrixboard.h"

TetrixBoard::TetrixBoard(QWidget *parent) : QFrame(parent),
    invisible(false),
    gamesPlayed(0),
    maxLinesRemoved(0),
    totalLinesRemoved(0),
    gameModel(BoardWidth, BoardHeight),
    tetris(&gameModel),
    locoBoss(BoardWidth),
    stochyBoss(0.2), //learning rate, put in named constant
    zuckerBoss(gameModel.getBoard().getNumFeatures()),
    boris(&zuckerBoss, &tetris),
    borisIsPlaying(true),
    borisInterval(0),
    autoPlay(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}

void TetrixBoard::setNextPieceLabel(QLabel *label)
{
    nextPieceLabel = label;
}

QSize TetrixBoard::sizeHint() const
{
    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}

QSize TetrixBoard::minimumSizeHint() const
{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void TetrixBoard::start(){
    emit gamesPlayedChanged(gamesPlayed);

    //update total lines
    if(tetris.getLinesRemoved() > 0){
        totalLinesRemoved += tetris.getLinesRemoved();
    }

    //update average
    emit avgLinesRemovedChanged(double(totalLinesRemoved) / double(gamesPlayed));

    //update maximum if appropriate
    if(tetris.getLinesRemoved() > maxLinesRemoved){
        maxLinesRemoved = tetris.getLinesRemoved();
        emit maxLinesRemovedChanged(maxLinesRemoved);
    }

    tetris.startNewGame();
    timer.start(tetris.getTimeoutTime(), this);
    borisTimer.start(borisInterval, this);
    refreshGUI();
    gamesPlayed++;

}

void TetrixBoard::pause(bool checked){
    tetris.setPaused(checked);
    update();
}

void TetrixBoard::setAISpeed(int speed){
    borisInterval = 2*(100 - speed);
    borisTimer.start(borisInterval,this);
}

void TetrixBoard::setAutoPlay(bool value){
    autoPlay = value;
    if(autoPlay){
        start();
    }
}

void TetrixBoard::setInvisiblePlay(bool value){
    invisible = value;
}



void TetrixBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    //draw the board
    const BoardModel& board = gameModel.getBoard();
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            const TetrixShape &shape = board.getShapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
    }

    //draw the tetronimo on top of the board
    const TetrixPiece &curPiece = gameModel.getCurrentPiece();
    const int curX = tetris.getCurrentPieceX();
    const int curY = tetris.getCurrentPieceY();
    if (curPiece.shape() != NoShape) {
        for (int i = 0; i < 4; ++i) {
            int x = curX + curPiece.x(i);
            int y = curY - curPiece.y(i);
            drawSquare(painter, rect.left() + x * squareWidth(),
                       boardTop + (BoardHeight - y - 1) * squareHeight(),
                       curPiece.shape());
        }
    }
}

void TetrixBoard::keyPressEvent(QKeyEvent *event)
{
    if (!tetris.isStarted() || tetris.isPaused()) {
        QFrame::keyPressEvent(event);
        return;
    }

    switch (event->key()) {
    case Qt::Key_Left:
        tetris.moveLeft();
        break;
    case Qt::Key_Right:
        tetris.moveRight();
        break;
    case Qt::Key_Down:
        tetris.moveDown();
        break;
    case Qt::Key_Up:
        tetris.rotateCCW();
        break;
    case Qt::Key_Space:
        tetris.drop();
        break;
    default:
        QFrame::keyPressEvent(event);
    }

    refreshGUI();
}

void TetrixBoard::timerEvent(QTimerEvent *event){
    if (event->timerId() == timer.timerId()) {
        tetris.timeoutElapsed();
        timer.start(tetris.getTimeoutTime(), this);
        if(!invisible)refreshGUI();
        if (!tetris.isStarted() && autoPlay){
            start();
        }
    } else if (event->timerId() == borisTimer.timerId()) {
        if(tetris.isStarted() && !tetris.isPaused() && borisIsPlaying){
            boris.tick();
            if(!invisible)refreshGUI();
        }
        if (!tetris.isStarted() && autoPlay){
            start();
        }
    } else {
        QFrame::timerEvent(event);
    }
}

void TetrixBoard::showNextPiece()
{
    if (!nextPieceLabel)
        return;

    const TetrixPiece& nextPiece = gameModel.getNextPiece();
    int dx = nextPiece.maxX() - nextPiece.minX() + 1;
    int dy = nextPiece.maxY() - nextPiece.minY() + 1;

    QPixmap pixmap(dx * squareWidth(), dy * squareHeight());
    QPainter painter(&pixmap);
    painter.fillRect(pixmap.rect(), nextPieceLabel->palette().background());

    for (int i = 0; i < 4; ++i) {
        int x = nextPiece.x(i) - nextPiece.minX();
        int y = nextPiece.y(i) - nextPiece.minY();
        drawSquare(painter, x * squareWidth(), y * squareHeight(),
                   nextPiece.shape());
    }
    nextPieceLabel->setPixmap(pixmap);
}

void TetrixBoard::refreshGUI(){
    emit levelChanged(tetris.getLevel());
    emit linesRemovedChanged(tetris.getLinesRemoved());
    update();
    showNextPiece();
}

void TetrixBoard::drawSquare(QPainter &painter, int x, int y, TetrixShape shape)
{
    static const QRgb colorTable[8] = {
        0x000000, 0xCC6666, 0x66CC66, 0x6666CC,
        0xCCCC66, 0xCC66CC, 0x66CCCC, 0xDAAA00
    };

    QColor color = colorTable[int(shape)];
    painter.fillRect(x + 1, y + 1, squareWidth() - 2, squareHeight() - 2,
                     color);

    painter.setPen(color.light());
    painter.drawLine(x, y + squareHeight() - 1, x, y);
    painter.drawLine(x, y, x + squareWidth() - 1, y);

    painter.setPen(color.dark());
    painter.drawLine(x + 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + squareHeight() - 1);
    painter.drawLine(x + squareWidth() - 1, y + squareHeight() - 1,
                     x + squareWidth() - 1, y + 1);
}
