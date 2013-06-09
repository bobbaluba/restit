#include "tetrisboard.h"
#include "tetriswindow.h"
#include "digitalclock.h"

#include <QtGui>

TetrisWindow::TetrisWindow()
{
    board = new TetrisBoard;

    nextPieceLabel = new QLabel;
    nextPieceLabel->setFrameStyle(QFrame::Box | QFrame::Raised);
    nextPieceLabel->setAlignment(Qt::AlignCenter);
    nextPieceLabel->setMinimumHeight(100);
    board->setNextPieceLabel(nextPieceLabel);

    levelLcd = new QLCDNumber(7);
    levelLcd->setSegmentStyle(QLCDNumber::Filled);
    linesLcd = new QLCDNumber(7);
    linesLcd->setSegmentStyle(QLCDNumber::Filled);

    QLCDNumber* tetrominoesPlayedLCD = new QLCDNumber(5);
    tetrominoesPlayedLCD->setSegmentStyle(QLCDNumber::Filled);

    DigitalClock* digitalClock = new DigitalClock();


    startButton = new QPushButton(tr("&Start"));
    startButton->setFocusPolicy(Qt::NoFocus);
    quitButton = new QPushButton(tr("&Quit"));
    quitButton->setFocusPolicy(Qt::NoFocus);
    pauseButton = new QPushButton(tr("&Pause"));
    pauseButton->setFocusPolicy(Qt::NoFocus);
    pauseButton->setCheckable(true);

    QGroupBox *aiSelector = new QGroupBox(tr("Select AI"));

    QRadioButton *noneRadio = new QRadioButton(tr("None"));
    QRadioButton *greedyRadio = new QRadioButton(tr("Greedy"));
    QRadioButton *reinforcementRadio = new QRadioButton(tr("Reinforcement"));
    reinforcementRadio->setChecked(true);

    QCheckBox *autoPlayCheckBox = new QCheckBox(tr("Auto Play"));
    QCheckBox *lineDownCheckBox = new QCheckBox(tr("Line Down Timeout"));
    lineDownCheckBox->setChecked(true);
    QCheckBox *invisiblePlayCheckBox = new QCheckBox(tr("Play without gui"));
    QCheckBox *lookAheadCheckBox = new QCheckBox(tr("Use lookahead"));
    lookAheadCheckBox->setChecked(true);

    QSlider *speedSlider = new QSlider(Qt::Horizontal, 0);
    speedSlider->setRange(0,100);
    speedSlider->setValue(100);


    QVBoxLayout *aiSelectorLayout = new QVBoxLayout;
    aiSelectorLayout->addWidget(noneRadio);
    aiSelectorLayout->addWidget(greedyRadio);
    aiSelectorLayout->addWidget(reinforcementRadio);
    aiSelectorLayout->addWidget(autoPlayCheckBox);
    aiSelectorLayout->addWidget(invisiblePlayCheckBox);
    aiSelectorLayout->addWidget(lineDownCheckBox);
    aiSelectorLayout->addWidget(lookAheadCheckBox);
    aiSelectorLayout->addWidget(speedSlider);

    //vbox->addStretch(1);

    aiSelector->setLayout(aiSelectorLayout);

    //statistics box
    QGroupBox *aiStatistics = new QGroupBox(tr("Statistics"));
    QLabel *numGamesDescription = new QLabel(tr("Games played"));
    QLCDNumber *numGames = new QLCDNumber(7);
    QLabel *totalMovesDescription = new QLabel(tr("Total moves"));
    QLCDNumber *totalMoves = new QLCDNumber(7);
    QLabel *maxLinesRemovedDescription = new QLabel(tr("Maximum lines removed"));
    QLCDNumber *maxLinesRemoved = new QLCDNumber(7);
    QLabel *avgLinesRemovedDescription = new QLabel(tr("Average lines removed"));
    QLCDNumber *avgLinesRemoved = new QLCDNumber(7);
    QLabel *movingAvgLinesRemovedDescription = new QLabel(tr("Moving average lines removed"));
    QLCDNumber *movingAvgLinesRemoved = new QLCDNumber(7);
    QGridLayout *aiStatisticsLayout = new QGridLayout;

    aiStatisticsLayout->addWidget(numGamesDescription, 0, 0);
    aiStatisticsLayout->addWidget(numGames, 0, 1);
    aiStatisticsLayout->addWidget(totalMovesDescription, 1, 0);
    aiStatisticsLayout->addWidget(totalMoves, 1, 1);
    aiStatisticsLayout->addWidget(maxLinesRemovedDescription, 2, 0);
    aiStatisticsLayout->addWidget(maxLinesRemoved, 2, 1);
    aiStatisticsLayout->addWidget(avgLinesRemovedDescription, 3, 0);
    aiStatisticsLayout->addWidget(avgLinesRemoved, 3, 1);
    aiStatisticsLayout->addWidget(movingAvgLinesRemovedDescription, 4, 0);
    aiStatisticsLayout->addWidget(movingAvgLinesRemoved, 4, 1);

    aiStatistics->setLayout(aiStatisticsLayout);

    //parameter loader
    QLineEdit *paramView = new QLineEdit("test");
    paramView->setReadOnly(true);
    QLineEdit *paramEdit = new QLineEdit;
    QGridLayout *paramBox = new QGridLayout;
    paramBox->addWidget(paramView, 0, 0);
    paramBox->addWidget(paramEdit, 1, 0);

    //tetris
    connect(startButton, SIGNAL(clicked()), board, SLOT(start()));
    connect(quitButton , SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(pauseButton, SIGNAL(toggled(bool)), board, SLOT(pause(bool)));
    //connect(board, SIGNAL(scoreChanged(int)), scoreLcd, SLOT(display(int)));
    connect(board, SIGNAL(levelChanged(int)), levelLcd, SLOT(display(int)));
    connect(board, SIGNAL(linesRemovedChanged(int)), linesLcd, SLOT(display(int)));

    //connect ai stuff
    connect(speedSlider, SIGNAL(valueChanged(int)), board, SLOT(setAISpeed(int)));
    connect(autoPlayCheckBox, SIGNAL(toggled(bool)), board, SLOT(setAutoPlay(bool)));
    connect(invisiblePlayCheckBox, SIGNAL(toggled(bool)), board, SLOT(setInvisiblePlay(bool)));
    connect(lookAheadCheckBox, SIGNAL(toggled(bool)), board, SLOT(setLookAheadEnabled(bool)));
    connect(lineDownCheckBox, SIGNAL(toggled(bool)), board, SLOT(setLineDownTimeoutEnabled(bool)));
    connect(board, SIGNAL(gamesPlayedChanged(int)), numGames, SLOT(display(int)));
    connect(board, SIGNAL(totalMovesChanged(int)), totalMoves, SLOT(display(int)));
    connect(board, SIGNAL(maxLinesRemovedChanged(int)), maxLinesRemoved, SLOT(display(int)));
    connect(board, SIGNAL(avgLinesRemovedChanged(double)), avgLinesRemoved, SLOT(display(double)));
    connect(board, SIGNAL(movingAvgLinesChanged(double)), movingAvgLinesRemoved, SLOT(display(double)));
    connect(board, SIGNAL(parametersChanged(QString)), paramView, SLOT(setText(QString)));
    connect(paramEdit, SIGNAL(textEdited(QString)), board, SLOT(setParameters(QString)));

    //connect ai selector
    connect(noneRadio, SIGNAL(toggled(bool)), board, SLOT(setNoAI(bool)));
    connect(reinforcementRadio, SIGNAL(toggled(bool)), board, SLOT(setZuckerAI(bool)));
    connect(greedyRadio, SIGNAL(toggled(bool)), board, SLOT(setGreedyAI(bool)));

    //tetris
    QGridLayout *tetrisLayout = new QGridLayout;
    tetrisLayout->addWidget(board, 0, 0, 11, 1);

    int y=0;
    tetrisLayout->addWidget(createLabel(tr("TIME ELAPSED")), y++, 1);
    tetrisLayout->addWidget(digitalClock, y++, 1);
    tetrisLayout->addWidget(createLabel(tr("LEVEL")), y++, 1);
    tetrisLayout->addWidget(levelLcd, y++, 1);
    tetrisLayout->addWidget(createLabel(tr("LINES")), y++, 1);
    tetrisLayout->addWidget(linesLcd, y++, 1);
    tetrisLayout->addWidget(createLabel(tr("NEXT")), y++, 1);
    tetrisLayout->addWidget(nextPieceLabel, y++, 1);
    tetrisLayout->addWidget(quitButton, y++, 1);
    tetrisLayout->addWidget(pauseButton, y++, 1);
    tetrisLayout->addWidget(startButton, y++, 1);
    tetrisLayout->setColumnStretch(0, 2);
    tetrisLayout->setColumnStretch(1, 1);

    //our ai stuff
    QGridLayout *AILayout = new QGridLayout;
    AILayout->addWidget(aiSelector, 1, 0);
    AILayout->addWidget(aiStatistics, 2, 0);

    //combine layouts
    QGridLayout *boxLayout = new QGridLayout;
    boxLayout->addLayout(tetrisLayout, 0, 0);
    boxLayout->setColumnStretch(0, 3);
    boxLayout->addLayout(AILayout, 0, 1);
    boxLayout->setColumnStretch(1, 1);
    boxLayout->addLayout(paramBox, 1, 0, 1, 2);

    setLayout(boxLayout);

    setWindowTitle(tr("Restit"));
    resize(768, 576);
}

QLabel *TetrisWindow::createLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setAlignment(Qt::AlignHCenter | Qt::AlignBottom);
    return lbl;
}
