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
    TetrixShape getShapeAt(int x, int y) const { return tiles[(y * width) + x]; }
    bool isFree(const TetrixPiece &piece, int x, int y) const;
    BoardModel placePiece(const TetrixPiece &piece, int x, int y, int* numLinesRemoved) const;
private:
    void setShapeAt(int x, int y, TetrixShape shape) {tiles[(y * width) + x] = shape; }
    int placePiece(const TetrixPiece &piece, int x, int y); //returns number of lines removed
    int removeFullLines();
};

#endif // BOARDMODEL_H
