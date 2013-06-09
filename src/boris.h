#ifndef BORIS_H
#define BORIS_H

#include <cstdlib>
#include <vector>

#include "tetrisagent.h"
#include "complextetris.h"

class Boris{
private:
    enum Action {
        MOVE_LEFT, MOVE_RIGHT, MOVE_DOWN, ROTATE_CCW, DROP
    };
private:
    TetrisAgent *boss;
    ComplexTetris *tetris;
    std::vector<Boris::Action> plan;
public:
    explicit Boris(TetrisAgent *boss, ComplexTetris* tetris);
    void tick(); //called whenever boris can do an action
    void updatePlan();
    Boris::Action getNextAction();
    void setBoss(TetrisAgent* boss);
    TetrisAgent* getBoss(){return boss;}
};

#endif // BORIS_H
