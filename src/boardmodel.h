#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include <tetrixpiece.h>

#include <vector>

class BoardModel
{
private:
    int width, height;
    std::vector<TetrixShape> tiles;
public:
    BoardModel(int width, int height);
    TetrixShape shapeAt(int x, int y) { return tiles[(y * width) + x]; }
    void setShapeAt(int x, int y, TetrixShape shape) {tiles[(y * width) + x] = shape; }
    bool isFree(const TetrixPiece &piece, int x, int y);
    void placePiece(const TetrixPiece &piece, int x, int y);
    int removeFullLines();
    void clear();
};

#endif // BOARDMODEL_H
