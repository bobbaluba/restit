#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

#include "bossofboris.h"

class Boris{
public:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, ROTATE_CCW, MOVE_DOWN, DROP
    };
private:
    std::vector<Boris::Action> plan;
    BossOfBoris *boss;
public:
    void updatePlan(const State& currentState);
    Boris(BossOfBoris *boss);
    Boris::Action getNextAction();
};

#endif // BORIS_H
