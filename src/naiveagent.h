#ifndef NAIVEAGENT_H
#define NAIVEAGENT_H

#include "tetrisagent.h"

class NaiveAgent : public TetrisAgent{
public:
    explicit NaiveAgent();
    virtual const SimpleAction getGoal(const State &currentState);
};

#endif // NAIVEAGENT_H
