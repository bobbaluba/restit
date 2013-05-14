#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
public:
    StochyBoss();
    virtual BorisGoal getGoal(const State &currentState);
};

#endif // STOCHYBOSS_H
