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
    //create a new empty board
    BoardModel(int width, int height);

    TetrixShape getShapeAt(int x, int y) const { return tiles[(y * width) + x]; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    //check if there are no conflicting or pieces, or pieces outside the board
    bool isFree(const TetrixPiece &piece, int x, int y) const;

    bool canDropIntoColumn(const TetrixPiece &piece, int x) const;

    //returns the board after dropping a piece into column x, number of lines removed are stored in *numLinesRemoved
    BoardModel dropPiece(const TetrixPiece &piece, int x, int* numLinesRemoved) const;

    //required if we plan on supporting placement of pieces under overhangs
    BoardModel placePiece(const TetrixPiece &piece, int x, int y, int* numLinesRemoved) const;

private:
    //non-deterministic helper methods
    void setShapeAt(int x, int y, TetrixShape shape) {tiles[(y * width) + x] = shape; }
    int placePiece(const TetrixPiece &piece, int x, int y); //returns number of lines removed
    int removeFullLines();
};

#endif // BOARDMODEL_H
