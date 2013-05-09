#ifndef BOSSOFBORIS_H
#define BOSSOFBORIS_H

#include "borisgoal.h"

//interaface

class BossOfBoris{
public:
    virtual BorisGoal getGoal() = 0;
};

#endif // BOSSOFBORIS_H
