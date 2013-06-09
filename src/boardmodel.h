#ifndef BOARDMODEL_H
#define BOARDMODEL_H

#include "simpleaction.h"

#include "tetronimo.h"

#include <vector>

class BoardModel
{
private:
    int width, height;
    std::vector<TetronimoShape> tiles;

public:

    //create a new empty board
    explicit BoardModel(int width, int height);

    TetronimoShape getShapeAt(int x, int y) const { return tiles[(y * width) + x]; }
    bool isEmpty(int x, int y) const { return getShapeAt(x,y) == NoShape; }

    int getWidth() const { return width; }
    int getHeight() const { return height; }

    //feature calculation
    int getHoles() const;
    int getColumnHeight(int x) const;
    int getColumnHeightDifference(int x1, int x2) const;
    int getMaximumHeight() const;
    const std::vector<double> getFeatures() const;
    size_t getNumFeatures() const {return getFeatures().size();}


    //check if there are no conflicting or pieces, or pieces outside the board
    bool isFree(const Tetronimo &piece, int x, int y) const;

    bool canDropIntoColumn(const Tetronimo &piece, int x) const;

    //returns the board after dropping a piece into column x, number of lines removed are stored in *numLinesRemoved
    const BoardModel dropPiece(const Tetronimo &piece, int x, int* numLinesRemoved) const;

    //required if we plan on supporting placement of pieces under overhangs
    const BoardModel placePiece(const Tetronimo &piece, int x, int y, int* numLinesRemoved) const;

    const BoardModel applyAction(const SimpleAction& action, const Tetronimo &piece, int *numLinesRemoved) const;
    const std::vector<SimpleAction> getLegalActions(const Tetronimo &currentPiece) const;

private:
    //non-deterministic helper methods
    void setShapeAt(int x, int y, const TetronimoShape &shape) {tiles[(y * width) + x] = shape; }
    int placePiece(const Tetronimo &piece, int x, int y); //returns number of lines removed
    int removeFullLines();
};

#endif // BOARDMODEL_H
