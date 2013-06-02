#ifndef LOCOBOSS_H
#define LOCOBOSS_H

//boris implements the strategy pattern, and this is his strategy

#include "bossofboris.h"

class LocoBoss : public BossOfBoris{
private:
    const int boardWidth;
public:
    explicit LocoBoss(int boardWidth);
    virtual SimpleAction getGoal(const State& currentState);
};

#endif // LOCOBOSS_H
