#include "stochyboss.h"

#include <vector>
#include <limits>


StochyBoss::StochyBoss(double learningRate) : learningRate(learningRate){
}

BorisGoal StochyBoss::getGoal(const State &currentState){

    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    double maxScore = std::numeric_limits<double>::lowest();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const BorisGoal& action = *it;

        std::vector<double> features = getFeatures(currentState, action);

        //create parameter vector
        std::vector<double> parameterVector = getParameterVector(features.size());

        double score = calculateQuality(parameterVector, features);

        if(score>maxScore){
            maxScore = score;
            bestAction = action;
        }
    }
    return bestAction;
}

double StochyBoss::calculateQuality(const std::vector<double> parameterVector, const std::vector<double> features){
    //evaluate state
    double score = 0;
    for(unsigned int i = 0; i < features.size(); ++i){
        score += parameterVector[i] * features[i];
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

std::vector<double> StochyBoss::getParameterVector(int size){
    if(parameterVector.empty()){
        //create parameter vector
        parameterVector.reserve(size);
        for(int i = 0; i<size; ++i){
            parameterVector.push_back(-1 + (double)rand()/((double)RAND_MAX/(1+1)));
        }
        //debug for a vector that performs reasonably well
        //set weight for numlinesremoved and number of holes
        //parameterVector[parameterVectro.size()-1] = 1; //linesremoved
        //parameterVector[parameterVectro.size()-2] = -1; //number of holes
    }
    return parameterVector;
}
