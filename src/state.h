#ifndef STATE_H
#define STATE_H

#include "borisgoal.h"
#include "boardmodel.h"

#include <vector>

class State{
private:
    BoardModel board;
    TetrixPiece currentPiece; //piece we are about to place
public:
    State(const BoardModel& board, const TetrixPiece& currentPiece):
        board(board),
        currentPiece(currentPiece)
    {}
    std::vector<BorisGoal> getLegalBorisGoals() const;
    BoardModel applyAction(BorisGoal borisGoal, int *numLinesRemoved) const;
};

#endif // STATE_H
