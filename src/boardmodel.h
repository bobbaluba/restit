#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <tetrixpiece.h>

#include <vector>

class BoardModel
{
private:
    std::vector<TetrixShape> tiles;
    int width, height;
public:
    BoardModel(int width, int height);
    void clear();
};

#endif // BOARDMODEL_H
