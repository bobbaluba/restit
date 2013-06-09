#include <QtGui>

#include <stdlib.h>

#include "tetriswindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    TetrisWindow window;
    window.show();
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    return app.exec();
}
