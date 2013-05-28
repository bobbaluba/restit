#ifndef STATE_H
#define STATE_H

#include "borisgoal.h"
#include "boardmodel.h"

#include <vector>

class State{
private:
    BoardModel board;
    TetrixPiece currentPiece; //piece we are about to place
    TetrixPiece nextPiece; //piece we are about to place
public:
    State(const BoardModel& board, const TetrixPiece& currentPiece, const TetrixPiece& nextPiece):
        board(board),
        currentPiece(currentPiece),
        nextPiece(nextPiece)
    {}
    std::vector<BorisGoal> getLegalActions() const;
    BoardModel applyAction(BorisGoal borisGoal, int *numLinesRemoved) const;
    const TetrixPiece& getCurrentPiece() const { return currentPiece; }
    const TetrixPiece& getNextPiece() const { return nextPiece; }
};

#endif // STATE_H
