#ifndef BOSSOFBORIS_H
#define BOSSOFBORIS_H

#include "simpleaction.h"
#include "state.h"

//interaface

class BossOfBoris{
public:
    virtual const SimpleAction getGoal(const State& currentState) = 0;
    virtual long getTotalMoves() const = 0;
};

#endif // BOSSOFBORIS_H
