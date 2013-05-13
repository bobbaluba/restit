#include "boris.h"

#include <cassert>

Boris::Boris(BossOfBoris *boss, ComplexTetris *tetris) :
    boss(boss),
    tetris(tetris){
}

void Boris::tick(){
    if(plan.empty()){
        updatePlan();
    }
    Boris::Action nextMove = plan.back();
    bool moveSuccessful = true;
    switch (nextMove) {
    case MOVE_LEFT:
        tetris->moveLeft();
        break;
    case MOVE_RIGHT:
        tetris->moveRight();
        break;
    case MOVE_DOWN:
        tetris->moveDown();
        break;
    case ROTATE_CCW:
        moveSuccessful = tetris->rotateCCW();
        break;
    case DROP:
        tetris->drop();
        break;
    }
    if(!moveSuccessful){
        plan.push_back(MOVE_DOWN); //move down while something is in the way
    } else {
        plan.pop_back();
    }
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

