#include "stochyboss.h"

#include <vector>
#include <limits>

StochyBoss::StochyBoss() {
}

BorisGoal StochyBoss::getGoal(const State &currentState){

    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    float maxScore = std::numeric_limits<float>::lowest();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        //check each move and number of lines removed
        const BorisGoal& action = *it;

        //gather features
        int linesRemoved;
        BoardModel nextBoard = currentState.applyAction(action, &linesRemoved);
        std::vector<int> features = nextBoard.getFeatures();
        features.push_back(linesRemoved);

        //create parameter vector
        std::vector<float> parameterVector(features.size(), 0);

        //set weight for numlinesremoved and number of holes
        parameterVector[features.size()-1] = 1; //linesremoved
        parameterVector[features.size()-2] = -1; //number of holes

        //evaluate state
        float score = 0;
        for(unsigned int i = 0; i < features.size(); ++i){
            score += parameterVector[i] * features[i];
        }

        if(score>maxScore){
            maxScore = score;
            bestAction = action;
        }
    }
    return bestAction;
}
