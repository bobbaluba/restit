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

const std::vector<double> BoardModel::getFeatures() const {
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

bool BoardModel::isFree(const Tetronimo &piece, int x, int y) const{
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

bool BoardModel::canDropIntoColumn(const Tetronimo& piece, int x) const{
    int y = height - 1 + piece.minY();
    return isFree(piece, x, y);
}

const BoardModel BoardModel::dropPiece(const Tetronimo &piece, int x, int *numLinesRemoved) const {
    int curY = height - 1 + piece.minY();

    assert(isFree(piece, x, curY)); //verify that the move is legal

    while(isFree(piece, x, curY-1)){
        --curY;
    }
    return placePiece(piece, x, curY, numLinesRemoved);
}

const BoardModel BoardModel::placePiece(const Tetronimo &piece, int x, int y, int *numLinesRemoved) const {
    BoardModel copy = *this;
    *numLinesRemoved = copy.placePiece(piece, x, y);
    return copy;
}

const BoardModel BoardModel::applyAction(const SimpleAction &action, const Tetronimo& piece, int *numLinesRemoved) const{
    Tetronimo tmpPiece = piece;
    for(int i=0; i<action.rotation; ++i){
        tmpPiece = tmpPiece.rotatedLeft();
    }
    return dropPiece(tmpPiece, action.position, numLinesRemoved);
}

const std::vector<SimpleAction> BoardModel::getLegalActions(const Tetronimo &currentPiece) const {
    std::vector<SimpleAction> legalActions;
    Tetronimo piece = currentPiece;
    for(int i=0; i<4; ++i){
        //check each x position
        for(int j = -piece.minX(); j < getWidth() - piece.maxX(); ++j){
            if(canDropIntoColumn(piece, j)){
                legalActions.push_back(SimpleAction{i, j});
            }
        }
        piece = piece.rotatedLeft();
    }
    return legalActions;
}

int BoardModel::placePiece(const Tetronimo &piece, int x, int y){
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


