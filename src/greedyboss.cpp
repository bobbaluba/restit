#include "greedyboss.h"

#include <cstdlib>

GreedyBoss::GreedyBoss(){

}

BorisGoal GreedyBoss::getGoal(const State &currentState){
    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    int maxNumLinesRemoved = -1;
    BorisGoal bestAction;
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        //check each move and number of lines removed
        const BorisGoal& action = *it;
        int linesRemoved;
        currentState.applyAction(action, &linesRemoved);
        if(linesRemoved > maxNumLinesRemoved){
            maxNumLinesRemoved=linesRemoved;
            bestAction = action;
        }
    }
    if(maxNumLinesRemoved==0){
        int actionIndex = rand() % actions.size();
        return actions[actionIndex];
    } else {
        return bestAction;
    }
}
