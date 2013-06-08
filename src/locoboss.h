#ifndef LOCOBOSS_H
#define LOCOBOSS_H

//boris implements the strategy pattern, and this is his strategy

#include "bossofboris.h"

class LocoBoss : public BossOfBoris{
private:
    const int boardWidth;
    long t;
public:
    explicit LocoBoss(int boardWidth);
    virtual const SimpleAction getGoal(const State& currentState);
    virtual long getTotalMoves() const {return t;}
};

#endif // LOCOBOSS_H
