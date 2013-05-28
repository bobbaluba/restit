#include "stochyboss.h"

#include <vector>
#include <limits>
#include <cassert>


StochyBoss::StochyBoss(){
}

BorisGoal StochyBoss::getGoal(const State &currentState){
    assert(!theta.empty());
    std::vector<BorisGoal> actions = currentState.getLegalActions();
    double maxScore = std::numeric_limits<double>::lowest();
    BorisGoal bestAction = actions[0];
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const BorisGoal& firstAction = *it;

        int numLinesRemoved;
        BoardModel nextBoard = currentState.applyAction(firstAction, &numLinesRemoved);
        std::vector<BorisGoal> secondActions = nextBoard.getLegalActions(currentState.getNextPiece());

        for(unsigned int i = 0; i<secondActions.size(); ++i){
            int numLinesRemovedSecond;
            BoardModel finalBoard = nextBoard.applyAction(secondActions[i], currentState.getNextPiece(), &numLinesRemovedSecond);
            int finalLinesRemoved = numLinesRemovedSecond + numLinesRemoved;

            Vector features = nextBoard.getFeatures();
            features.push_back(finalLinesRemoved);

            double score = calculateQuality(theta, features);

            if(score>maxScore){
                maxScore = score;
                bestAction = firstAction;
            }
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

//not used?
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
