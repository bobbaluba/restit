#ifndef GREEDYBOSS_H
#define GREEDYBOSS_H

#include "bossofboris.h"

class GreedyBoss : public BossOfBoris{
public:
    GreedyBoss();
    virtual BorisGoal getGoal(const State &currentState);
};

#endif // GREEDYBOSS_H
