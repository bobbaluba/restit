#include "boris.h"

#include "tetrixboard.h"

void Boris::getNextGoal() {
    //TODO: do something useful instead
    goalPosition = rand() % TetrixBoard::BoardWidth;
    goalRotation = rand() % 4;
}

Boris::Boris(){
}

Boris::Action Boris::getNextAction()
{
    if(plan.empty()){
        getNextGoal();
        const int startPosition = TetrixBoard::getStartColumn();
        int dx = goalPosition - startPosition;

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
        for(int i=0; i<goalRotation; ++i){
            plan.push_back(ROTATE_CCW);
        }
    }
    Boris::Action nextMove = plan.back();
    plan.pop_back();
    return nextMove;
}
