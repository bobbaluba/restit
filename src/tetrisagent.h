#ifndef TETRISAGENT_H
#define TETRISAGENT_H

#include "simpleaction.h"
#include "state.h"

//interaface

class TetrisAgent{
public:
    virtual const SimpleAction getGoal(const State& currentState) = 0;
    virtual long getTotalMoves() const = 0;
};

#endif // TETRISAGENT_H
