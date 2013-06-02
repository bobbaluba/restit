#ifndef BOSSOFBORIS_H
#define BOSSOFBORIS_H

#include "simpleaction.h"
#include "state.h"

//interaface

class BossOfBoris{
public:
    virtual SimpleAction getGoal(const State& currentState) = 0;
};

#endif // BOSSOFBORIS_H
