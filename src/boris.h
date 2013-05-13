#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

#include "bossofboris.h"
#include "complextetris.h"

class Boris{
public:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, ROTATE_CCW, DROP
    };
private:
    BossOfBoris *boss;
    ComplexTetris* tetris;
    std::vector<Boris::Action> plan;
public:
    Boris(BossOfBoris *boss, ComplexTetris* tetris);
    void updatePlan();
    Boris::Action getNextAction();
};

#endif // BORIS_H
