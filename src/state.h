#ifndef STATE_H
#define STATE_H

#include "borisgoal.h"
#include "boardmodel.h"

#include <vector>

class State{
private:
    TetrixPiece currentPiece; //piece we are about to place
    BoardModel board;
public:
    State(const TetrixPiece& currentPiece, const BoardModel& board): currentPiece(currentPiece), board(board){}
    std::vector<BorisGoal> getLegalBorisGoals() const;
    BoardModel applyAction(BorisGoal borisGoal, int *numLinesRemoved) const;
};

#endif // STATE_H
