#ifndef RANDOMAGENT_H
#define RANDOMAGENT_H

//boris implements the strategy pattern, and this is his strategy

#include "tetrisagent.h"

class RandomAgent : public TetrisAgent{
private:
    const int boardWidth;
public:
    explicit RandomAgent(int boardWidth);
    virtual const SimpleAction getGoal(const State& currentState);
};

#endif // RANDOMAGENT_H
