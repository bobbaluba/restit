#include "greedyboss.h"

#include <cstdlib>

GreedyBoss::GreedyBoss(){

}

BorisGoal GreedyBoss::getGoal(const State &currentState){
    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    int maxNumLinesRemoved;
    int minimumNumberOfHoles = currentState.applyAction(actions[0], &maxNumLinesRemoved).getHoles();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        //check each move and number of lines removed
        const BorisGoal& action = *it;
        int linesRemoved;
        int holes = currentState.applyAction(action, &linesRemoved).getHoles();
        if(holes<minimumNumberOfHoles){
            minimumNumberOfHoles = holes;
            maxNumLinesRemoved = linesRemoved;
            bestAction = action;
        } else if (holes == minimumNumberOfHoles){
            if(linesRemoved > maxNumLinesRemoved){
                maxNumLinesRemoved = linesRemoved;
                bestAction = action;
            }
        }
    }
    return bestAction;
}
