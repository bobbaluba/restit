#include "boris.h"

#include <cassert>

Boris::Boris(BossOfBoris *boss, ComplexTetris *tetris) :
    boss(boss),
    tetris(tetris){
}

void Boris::updatePlan() {
    plan.clear();

    State currentState = tetris->getState();
    BorisGoal goal = boss->getGoal(currentState);

    const int currentX = tetris->getCurrentPieceX();
    int dx = goal.position - currentX;

    plan.push_back(DROP);

    //column alignment
    Boris::Action direction = MOVE_RIGHT;
    if(dx<0){
        direction = MOVE_LEFT;
        dx *= -1;
    }
    for(int i=0; i<dx; ++i){
        plan.push_back(direction);
    }

    //rotation
    for(int i=0; i<goal.rotation; ++i){
        plan.push_back(ROTATE_CCW);
    }
}

Boris::Action Boris::getNextAction(){
    assert(!plan.empty());
    Boris::Action nextMove = plan.back();
    plan.pop_back();
    return nextMove;
}
