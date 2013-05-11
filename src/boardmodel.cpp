#include "boardmodel.h"

#include <cassert>

BoardModel::BoardModel(int width, int height): width(width), height(height), tiles(width*height, NoShape){
}

bool BoardModel::isFree(const TetrixPiece &piece, int x, int y) const{
    for (int i = 0; i < 4; ++i) {
        int currentX = x + piece.x(i);
        int currentY = y - piece.y(i);
        if (currentX < 0 || currentX >= width || currentY < 0 || currentY >= height)
            return false;
        if (getShapeAt(currentX, currentY) != NoShape)
            return false;
    }
    return true;
}

bool BoardModel::canDropIntoColumn(const TetrixPiece& piece, int x) const{
    int y = height - 1 + piece.minY();
    return isFree(piece, x, y);
}

BoardModel BoardModel::dropPiece(const TetrixPiece &piece, int x, int *numLinesRemoved) const {
    int curY = height - 1 + piece.minY();

    assert(isFree(piece, x, curY)); //verify that the move is legal

    while(isFree(piece, x, curY-1)){
        --curY;
    }
    return placePiece(piece, x, curY, numLinesRemoved);
}

BoardModel BoardModel::placePiece(const TetrixPiece &piece, int x, int y, int *numLinesRemoved) const {
    BoardModel copy = *this;
    *numLinesRemoved = copy.placePiece(piece, x, y);
    return copy;
}

int BoardModel::placePiece(const TetrixPiece &piece, int x, int y){
    for (int i = 0; i < 4; ++i) {
        int currentX = x + piece.x(i);
        int currentY = y - piece.y(i);
        setShapeAt(currentX, currentY, piece.shape());
    }
    return removeFullLines();
}

int BoardModel::removeFullLines(){
    int numFullLines = 0;

    for (int i = height - 1; i >= 0; --i) {
        bool lineIsFull = true;

        for (int j = 0; j < width; ++j) {
            if (getShapeAt(j, i) == NoShape) {
                lineIsFull = false;
                break;
            }
        }

        if (lineIsFull) {
            ++numFullLines;
            for (int k = i; k < height - 1; ++k) {
                for (int j = 0; j < width; ++j)
                    setShapeAt(j, k, getShapeAt(j, k + 1));
            }
            for (int j = 0; j < width; ++j)
                setShapeAt(j, height - 1, NoShape);
        }
    }

    return numFullLines;
}


