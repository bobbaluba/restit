#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

#include "bossofboris.h"
#include "complextetris.h"

class Boris{
private:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, ROTATE_CCW, DROP
    };
private:
    BossOfBoris *boss;
    ComplexTetris *tetris;
    std::vector<Boris::Action> plan;
public:
    explicit Boris(BossOfBoris *boss, ComplexTetris* tetris);
    void tick(); //called whenever boris can do an action
    void updatePlan();
    Boris::Action getNextAction();
    void setBoss(BossOfBoris* boss);
    BossOfBoris* getBoss(){return boss;}
};

#endif // BORIS_H
