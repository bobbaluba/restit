#include "greedyboss.h"

#include <cstdlib>

GreedyBoss::GreedyBoss(){

}

const SimpleAction GreedyBoss::getGoal(const State &currentState){
    std::vector<SimpleAction> actions = currentState.getLegalActions();
    int maxNumLinesRemoved;
    int minimumNumberOfHoles = currentState.applyAction(actions.at(0), &maxNumLinesRemoved).getHoles();
    SimpleAction bestAction = actions.at(0);
    for(std::vector<SimpleAction>::iterator it = actions.begin(); it!=actions.end(); ++it){
        //check each move and number of lines removed
        const SimpleAction& action = *it;
        int linesRemoved;
        int holes = currentState.applyAction(action, &linesRemoved).getHoles();
        if(linesRemoved>maxNumLinesRemoved){
            maxNumLinesRemoved = linesRemoved;
            minimumNumberOfHoles = holes;
            bestAction = action;
        } else if (linesRemoved == maxNumLinesRemoved){
            if(holes < minimumNumberOfHoles){
                bestAction = action;
                minimumNumberOfHoles = holes;
            }
        }
    }
    return bestAction;
}
