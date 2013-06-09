#ifndef TETRISAGENT_H
#define TETRISAGENT_H

#include "simpleaction.h"
#include "state.h"

//abstract class

class TetrisAgent{
private:
    bool lookAhead;
protected:
    long t;
public:
    TetrisAgent():lookAhead(true), t(0){}
    virtual const SimpleAction getGoal(const State& currentState) = 0;
    virtual long getTotalMoves() const { return t; }
    virtual bool isUsinglookAhead() const {return lookAhead;}
    virtual void setLookAheadEnabled(bool enabled){ lookAhead = enabled; }
};

#endif // TETRISAGENT_H
