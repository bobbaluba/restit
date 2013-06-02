#ifndef STATE_H
#define STATE_H

#include "simpleaction.h"
#include "boardmodel.h"

#include <vector>

class State{
private:
    BoardModel board;
    TetrixPiece currentPiece; //piece we are about to place
    TetrixPiece nextPiece; //piece we are about to place
public:
    explicit State(const BoardModel& board, const TetrixPiece& currentPiece, const TetrixPiece& nextPiece):
        board(board),
        currentPiece(currentPiece),
        nextPiece(nextPiece)
    {}
    const std::vector<SimpleAction> getLegalActions() const;
    const BoardModel applyAction(const SimpleAction& action, int *numLinesRemoved) const;
    const TetrixPiece& getCurrentPiece() const { return currentPiece; }
    const TetrixPiece& getNextPiece() const { return nextPiece; }
};

#endif // STATE_H
