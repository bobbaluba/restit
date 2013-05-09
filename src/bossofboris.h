#ifndef BOSSOFBORIS_H
#define BOSSOFBORIS_H

#include "borisgoal.h"
#include "state.h"

//interaface

class BossOfBoris{
public:
    virtual BorisGoal getGoal(const State& currentState) = 0;
};

#endif // BOSSOFBORIS_H
