#include "boardmodel.h"

#include <cassert>

BoardModel::BoardModel(int width, int height):
    width(width),
    height(height),
    tiles(width*height, NoShape){
}

int BoardModel::getHoles() const{
    int numHoles = 0;
    for(int x=0; x<width; ++x){
        bool hasSeenNonEmpty = false;
        for(int y = height-1; y >= 0; --y){
            if(!isEmpty(x, y)){
                hasSeenNonEmpty=true;
            } else if (hasSeenNonEmpty) {
                ++numHoles;
            }
        }
    }
    return numHoles;
}

int BoardModel::getColumnHeight(int x) const {
    for(int y = height-1; y>=0; --y){
        if(!isEmpty(x,y)){
            return y;
        }
    }
    return 0;
}

int BoardModel::getColumnHeightDifference(int x1, int x2) const {
    return abs(getColumnHeight(x1) - getColumnHeight(x2));
}

int BoardModel::getMaximumHeight() const {
    for(int y = height - 1; y>=0; --y){
        for(int x = 0; x < width; ++x){
            if(!isEmpty(x,y)){
                return y;
            }
        }
    }
    return 0;
}

std::vector<double> BoardModel::getFeatures() const {
    std::vector<double> features;
    for(int i = 0; i < width; ++i){
        features.push_back(getColumnHeight(i));
    }
    for(int i = 0; i < width-1; ++i){
        features.push_back(getColumnHeightDifference(i, i+1));
    }
    features.push_back(getMaximumHeight());
    features.push_back(getHoles());
    return features;
}

bool BoardModel::isFree(const TetrixPiece &piece, int x, int y) const{
    for (int i = 0; i < 4; ++i) {
        int currentX = x + piece.x(i);
        int currentY = y - piece.y(i);
        if (currentX < 0 || currentX >= width || currentY < 0 || currentY >= height)
            return false;
        if (!isEmpty(currentX, currentY))
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
            if (isEmpty(j, i)) {
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


