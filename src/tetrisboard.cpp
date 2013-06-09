#include "tetrisboard.h"

#include <QtGui>

    #include <cassert>
#include <sstream>


TetrisBoard::TetrisBoard(QWidget *parent) : QFrame(parent),
    invisible(false),
    lineDownTimeoutEnabled(true),
    gamesPlayed(0),
    maxLinesRemoved(0),
    totalLinesRemoved(0),
    gameModel(BoardWidth, BoardHeight),
    tetris(&gameModel),
    gameOver(false),
    randomAgent(BoardWidth),
    stochyAgent(),
    zuckerAgent(gameModel.getBoard().getNumFeatures()),
    boris(&zuckerAgent, &tetris),
    borisIsPlaying(true),
    borisInterval(0),
    autoPlay(false)
{
    setFrameStyle(QFrame::Panel | QFrame::Sunken);
    setFocusPolicy(Qt::StrongFocus);
}

void TetrisBoard::setNextPieceLabel(QLabel *label)
{
    nextPieceLabel = label;
}

QSize TetrisBoard::sizeHint() const
{
    return QSize(BoardWidth * 15 + frameWidth() * 2,
                 BoardHeight * 15 + frameWidth() * 2);
}

QSize TetrisBoard::minimumSizeHint() const
{
    return QSize(BoardWidth * 5 + frameWidth() * 2,
                 BoardHeight * 5 + frameWidth() * 2);
}

void TetrisBoard::start(){
    gameOver = false;
    tetris.startNewGame();
    timer.start(tetris.getTimeoutTime(), this);
    borisTimer.start(borisInterval, this);
    refreshGUI();
}

void TetrisBoard::onGameOver(){
    gamesPlayed++;
    emit gamesPlayedChanged(gamesPlayed);
    //update total lines
    if(tetris.getLinesRemoved() > 0){
        totalLinesRemoved += tetris.getLinesRemoved();
    }

    //update average
    double currentAvg = double(totalLinesRemoved) / double(gamesPlayed);
    emit avgLinesRemovedChanged(currentAvg);

    //update moving average
    lastLinesRemoved[gamesPlayed % NumGamesMovingAverage] = tetris.getLinesRemoved();
    if(gamesPlayed >= 10){
        double sumAvgGames = 0;
        for(int i = 0; i < NumGamesMovingAverage; ++i){
            sumAvgGames += lastLinesRemoved[i];
        }
        double movingAvg = sumAvgGames/double(NumGamesMovingAverage);
        emit movingAvgLinesChanged(movingAvg);
    }


    //update maximum if appropriate
    if(tetris.getLinesRemoved() > maxLinesRemoved){
        maxLinesRemoved = tetris.getLinesRemoved();
        emit maxLinesRemovedChanged(maxLinesRemoved);
    }
}

void TetrisBoard::pause(bool checked){
    tetris.setPaused(checked);
    update();
}

void TetrisBoard::setAISpeed(int speed){
    borisInterval = 2*(100 - speed);
    borisTimer.start(borisInterval,this);
}

void TetrisBoard::setAutoPlay(bool value){
    autoPlay = value;
    if(autoPlay){
        start();
    }
}

void TetrisBoard::setInvisiblePlay(bool value){
    invisible = value;
}

void TetrisBoard::setParameters(QString newParameters){
    std::stringstream ss;
    ss << newParameters.toStdString();
    Vector newTheta;
    ss >> newTheta;
    zuckerAgent.setTheta(newTheta);
    stochyAgent.setTheta(newTheta);
    emit parametersChanged(newParameters);
}

void TetrisBoard::setNoAI(bool enabled){
    if(enabled){
        boris.setBoss(NULL);
    }
}

void TetrisBoard::setZuckerAI(bool enabled){
    if(enabled){
        boris.setBoss(&zuckerAgent);
    }
}

void TetrisBoard::setLineDownTimeoutEnabled(bool enabled){
    lineDownTimeoutEnabled = enabled;
}

void TetrisBoard::setLookAheadEnabled(bool enabled){
    stochyAgent.setLookAheadEnabled(enabled);
    zuckerAgent.setLookAheadEnabled(enabled);
    randomAgent.setLookAheadEnabled(enabled);
}

void TetrisBoard::setGreedyAI(bool enabled){
    if(enabled){
        stochyAgent.setTheta(zuckerAgent.getTheta());
        boris.setBoss(&stochyAgent);
    }
}


void TetrisBoard::paintEvent(QPaintEvent *event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);
    QRect rect = contentsRect();

    int boardTop = rect.bottom() - BoardHeight*squareHeight();

    //draw the board
    const BoardModel& board = gameModel.getBoard();
    for (int i = 0; i < BoardHeight; ++i) {
        for (int j = 0; j < BoardWidth; ++j) {
            const TetronimoShape &shape = board.getShapeAt(j, BoardHeight - i - 1);
            if (shape != NoShape)
                drawSquare(painter, rect.left() + j * squareWidth(),
                           boardTop + i * squareHeight(), shape);
        }
    }

    //draw the tetronimo on top of the board
    const Tetronimo &curPiece = gameModel.getCurrentPiece();
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

void TetrisBoard::keyPressEvent(QKeyEvent *event)
{
    if (!tetris.hasStarted() || tetris.isPaused()) {
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

void TetrisBoard::timerEvent(QTimerEvent *event){
    if (event->timerId() == timer.timerId()) {
        if(!tetris.isPaused()){
            if(lineDownTimeoutEnabled)tetris.timeoutElapsed();
            timer.start(tetris.getTimeoutTime(), this);
            if(!invisible)refreshGUI();
        }
    } else if (event->timerId() == borisTimer.timerId()) {
        if(tetris.hasStarted() && !tetris.isGameOver() && !tetris.isPaused() && borisIsPlaying){
            boris.tick();
            if(!invisible)refreshGUI();
        }
        if (!tetris.hasStarted() && autoPlay){
            start();
        }
    } else {
        QFrame::timerEvent(event);
    }
    if(tetris.isGameOver() && !gameOver){
        gameOver = true;
        onGameOver();
        if (autoPlay){
            start();
        }
    }
}

void TetrisBoard::showNextPiece()
{
    if (!nextPieceLabel)
        return;

    const Tetronimo& nextPiece = gameModel.getNextPiece();
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

void TetrisBoard::refreshGUI(){
    emit levelChanged(tetris.getLevel());
    emit linesRemovedChanged(tetris.getLinesRemoved());

    Vector parameters(zuckerAgent.getTheta());
    std::stringstream ss;
    ss << parameters;
    QString parametersString(ss.str().c_str());
    emit parametersChanged(parametersString);
    if(boris.getBoss()!=nullptr){
        emit totalMovesChanged(static_cast<int>(boris.getBoss()->getTotalMoves()));
    }

    update();
    showNextPiece();
}

void TetrisBoard::drawSquare(QPainter &painter, int x, int y, TetronimoShape shape)
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
