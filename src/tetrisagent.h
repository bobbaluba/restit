#ifndef TETRISAGENT_H
#define TETRISAGENT_H

#include "simpleaction.h"
#include "state.h"

//abstract class

class TetrisAgent{
protected:
    long t;
public:
    TetrisAgent():t(0){}
    virtual const SimpleAction getGoal(const State& currentState) = 0;
    virtual long getTotalMoves() const { return t; }
};

#endif // TETRISAGENT_H
