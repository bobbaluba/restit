#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

#include "bossofboris.h"

class Boris{
public:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, ROTATE_CCW, DROP
    };
private:
    std::vector<Boris::Action> plan;
    void makeNewPlan(const State& currentState);
    BossOfBoris *boss;
public:
    Boris(BossOfBoris *boss);
    Boris::Action getNextAction(const State& currentState);
};

#endif // BORIS_H
