#include "boris.h"

#include "tetrixboard.h"

void Boris::makeNewPlan() {
    BorisGoal goal = boss->getGoal();

    const int startPosition = TetrixBoard::getStartColumn();
    int dx = goal.position - startPosition;

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

Boris::Boris(BossOfBoris *boss) : boss(boss){
}

Boris::Action Boris::getNextAction()
{
    if(plan.empty()){
        makeNewPlan();
    }
    Boris::Action nextMove = plan.back();
    plan.pop_back();
    return nextMove;
}
