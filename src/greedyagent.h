#ifndef GREEDYAGENT_H
#define GREEDYAGENT_H

#include "tetrisagent.h"

class GreedyAgent : public TetrisAgent{
public:
    explicit GreedyAgent();
    virtual const SimpleAction getGoal(const State &currentState);
};

#endif // GREEDYAGENT_H
