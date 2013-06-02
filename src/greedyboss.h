#ifndef GREEDYBOSS_H
#define GREEDYBOSS_H

#include "bossofboris.h"

class GreedyBoss : public BossOfBoris{
public:
    explicit GreedyBoss();
    virtual const SimpleAction getGoal(const State &currentState);
};

#endif // GREEDYBOSS_H
