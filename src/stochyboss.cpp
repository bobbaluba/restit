#include "stochyboss.h"

#include <vector>
#include <limits>


StochyBoss::StochyBoss(){
}

BorisGoal StochyBoss::getGoal(const State &currentState){

    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    double maxScore = std::numeric_limits<double>::lowest();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const BorisGoal& action = *it;

        Vector features = getFeatures(currentState, action);

        //create parameter vector
        Vector parameterVector = createParameterVector(features.size());

        double score = calculateQuality(parameterVector, features);

        if(score>maxScore){
            maxScore = score;
            bestAction = action;
        }
    }
    return bestAction;
}

double StochyBoss::calculateQuality(const Vector& theta, const Vector& features){
    //evaluate state
    double score = 0;
    for(unsigned int i = 0; i < features.size(); ++i){
        score += theta[i] * features[i];
    }
    return score;
}

std::vector<double> StochyBoss::getFeatures(const State& currentState, const BorisGoal& action){
    //gather features
    int linesRemoved;
    BoardModel nextBoard = currentState.applyAction(action, &linesRemoved);
    std::vector<double> features = nextBoard.getFeatures();
    features.push_back(linesRemoved);
    return features;
}

std::vector<double> StochyBoss::createParameterVector(int size){
    if(theta.empty()){
        //create parameter vector
        theta.reserve(size);
        for(int i = 0; i<size; ++i){
            theta.push_back(-1 + (double)rand()/((double)RAND_MAX/(1+1)));
        }
    }
    return theta;
}
