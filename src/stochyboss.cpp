#include "stochyboss.h"

#include <vector>
#include <limits>


StochyBoss::StochyBoss(float learningRate) : learningRate(learningRate){
}

BorisGoal StochyBoss::getGoal(const State &currentState){

    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    float maxScore = std::numeric_limits<float>::lowest();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const BorisGoal& action = *it;

        std::vector<int> features = getFeatures(currentState, action);

        //create parameter vector
        std::vector<float> parameterVector = getParameterVector(features.size());

        float score = calculateQuality(parameterVector, features);

        if(score>maxScore){
            maxScore = score;
            bestAction = action;
        }
    }
    return bestAction;
}

void StochyBoss::updateWeights(const std::vector<float> &delta){
    for(unsigned int i=0; i<parameterVector.size(); ++i){
        parameterVector[i] += learningRate*delta[i];
    }
}

float StochyBoss::calculateQuality(const std::vector<float> parameterVector, const std::vector<int> features){
    //evaluate state
    float score = 0;
    for(unsigned int i = 0; i < features.size(); ++i){
        score += parameterVector[i] * features[i];
    }
    return score;
}

float StochyBoss::z(){
    const float discount = 0.7f;

}


std::vector<int> StochyBoss::getFeatures(const State& currentState, const BorisGoal& action){
    //gather features
    int linesRemoved;
    BoardModel nextBoard = currentState.applyAction(action, &linesRemoved);
    std::vector<int> features = nextBoard.getFeatures();
    features.push_back(linesRemoved);
    return features;
}

std::vector<float> StochyBoss::getParameterVector(int size){
    if(parameterVector.empty()){
        //create parameter vector
        parameterVector.reserve(size);
        for(int i = 0; i<size; ++i){
            parameterVector.push_back(-1 + (float)rand()/((float)RAND_MAX/(1+1)));
        }
        //debug for a vector that performs reasonably well
        //set weight for numlinesremoved and number of holes
        //parameterVector[22-1] = 1; //linesremoved
        //parameterVector[22-2] = -1; //number of holes
    }
    return parameterVector;
}
