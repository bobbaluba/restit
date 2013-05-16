#include "zuckermaas.h"

#include <cassert>
#include <limits>
#include <cmath>


float Z(const std::vector<float> theta, const State& x);
std::vector<float> grad_Z(const std::vector<float> theta, const State& x);
std::vector<BorisGoal> U(const State& x);
float l(const std::vector<float> theta, const State& x, const BorisGoal& u);
std::vector<float> grad_l(const std::vector<float> theta, const State& x, const BorisGoal& u);
float Q(const std::vector<float> theta, std::vector<float> features);
std::vector<float> grad_Q(const std::vector<float> theta, std::vector<float> features);
float q(const std::vector<float> theta, const State& x, const BorisGoal& u);
std::vector<float> grad_q(const std::vector<float> &theta, const State& x, const BorisGoal &u);
std::vector<float> f(const State& x, const BorisGoal& u);
float r(const State& x, const BorisGoal& u);
std::vector<float> z_tplus1(const std::vector<float> &z, float beta, const std::vector<float> &theta, const State& xtplus1, const BorisGoal &utplus1);
std::vector<float> delta_tplus1(const std::vector<float> &ztplus1, std::vector<float> deltat, const State& xtplus1, const BorisGoal& utplus1, float t);


float operator* (std::vector<float>& lhs, std::vector<float>& rhs){
    assert(lhs.size() == rhs.size());
    float sum = 0;
    for(unsigned int i=0; i<lhs.size(); ++i){
        sum += lhs[i] * rhs[i];
    }
    return sum;
}

std::vector<float> operator+ (const std::vector<float>& lhs, const std::vector<float>& rhs){
    assert(lhs.size() == rhs.size());
    std::vector<float> copy = lhs;
    for(unsigned int i=0; i<rhs.size(); ++i){
        copy[i] += rhs[i];
    }
    return copy;
}

std::vector<float> operator*(const std::vector<float>& lhs, float rhs){
    std::vector<float> copy = lhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= rhs;
    }
    return copy;
}

std::vector<float> operator/(const std::vector<float>& lhs, float rhs){
    return lhs * (1/rhs);
}

std::vector<float> operator*(float lhs, const std::vector<float>& rhs){
    return rhs * lhs;
}

std::vector<float> operator- (const std::vector<float>& rhs){
    std::vector<float> copy = rhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= -1;
    }
    return copy;
}

std::vector<float> operator- (const std::vector<float>& lhs, const std::vector<float>& rhs){
    return lhs + (-rhs);
}
ZuckerMaas::ZuckerMaas(float alpha):zt(22, 0), delta(22, 0), alpha(alpha), beta(0), t(0){
}

BorisGoal ZuckerMaas::getGoal(const State &currentState){
    std::vector<BorisGoal> actions = currentState.getLegalBorisGoals();
    float maxScore = std::numeric_limits<float>::lowest();
    BorisGoal bestAction = actions[0];
    std::vector<float> featuresOfChosenAction; //not needed ?
    for(std::vector<BorisGoal>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const BorisGoal& action = *it;

        std::vector<float> features = f(currentState, action);

        //create parameter vector
        std::vector<float> parameterVector = getParameterVector(features.size());

        float score = calculateQuality(parameterVector, features);

        if(score>maxScore){
            maxScore = score;
            bestAction = action;
            featuresOfChosenAction = features;
        }
    }
    //reinforce step
    //updateWeights(currentState, bestAction);

    const BorisGoal &utplus1 = bestAction;
    std::vector<float> &theta = parameters;
    const State& xtplus1 = currentState;

    zt = z_tplus1(zt,beta,theta,xtplus1,utplus1); //not currentState?
    delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

    theta = theta + (alpha * delta);

    ++t;
    return bestAction;
}

//Z
float Z(const std::vector<float> theta, const State& x){
    std::vector<BorisGoal> Us = U(x);
    float sum = 0;
    for(unsigned int i=0; i< Us.size(); ++i){
        sum += l(theta, x, Us[i]);
    }
    return sum;
}

//grad Z
std::vector<float> grad_Z(const std::vector<float> theta, const State& x){
    std::vector<BorisGoal> Us = U(x);
    std::vector<float> gradZ(22, 0);
    for(unsigned int j=0; j<Us.size(); ++j){
        std::vector<float> gradlj = grad_l(theta, x, Us[j]);
        for(unsigned int i=0; i<gradlj.size(); ++i){
            gradZ[i] += gradlj[i];
        }
    }
    return gradZ;
}

//U
std::vector<BorisGoal> U(const State& x){
    return x.getLegalBorisGoals();
}

//l
float l(const std::vector<float> theta, const State& x, const BorisGoal& u){
    return exp(Q(theta, f(x, u)));
}

//grad l
std::vector<float> grad_l(const std::vector<float> theta, const State& x, const BorisGoal& u){
    //ugly way of multiplying scalar by a vector
    std::vector<float> gradl = grad_Q(theta, f(x, u));
    float scalar = l(theta, x, u);
    for(unsigned int i = 0; i<gradl.size(); ++i){
        gradl[i] *= scalar;
    }
    return gradl;
}

//Q
float Q(const std::vector<float> theta, std::vector<float> features){
    float sum = 0;
    for(unsigned int i = 0; i<features.size(); ++i){
        sum += theta[i] * features[i];
    }
    return sum;
}

//grad Q
std::vector<float> grad_Q(const std::vector<float> theta, std::vector<float> features){
    return features;
}

//q
float q(const std::vector<float> theta, const State& x, const BorisGoal& u){
    return l(theta, x, u)/Z(theta, x);
}


//grad q
std::vector<float> grad_q(const std::vector<float> &theta, const State& x, const BorisGoal &u){
    return -1.0f/float(pow(Z(theta, x),2))*grad_Z(theta, x) * l(theta, x, u) + grad_l(theta, x, u) / Z(theta, x);
}

//f
std::vector<float> f(const State& x, const BorisGoal& u){
    //gather features
    int linesRemoved;
    BoardModel nextBoard = x.applyAction(u, &linesRemoved);
    std::vector<float> features = nextBoard.getFeatures();
    features.push_back(float(linesRemoved));
    return features;
}

//r - probably not needed
float r(const State& x, const BorisGoal& u){
    int numLinesRemoved;
    x.applyAction(u, &numLinesRemoved);
    return numLinesRemoved;
}

//z_t+1
std::vector<float> z_tplus1(const std::vector<float> &z, float beta, const std::vector<float> &theta, const State& xtplus1, const BorisGoal &utplus1){
    return beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1);
}

//delta_t+1
std::vector<float> delta_tplus1(const std::vector<float> &ztplus1, std::vector<float> deltat, const State& xtplus1, const BorisGoal& utplus1, float t){
    return deltat + t/(t+1) * (r(xtplus1, utplus1)*ztplus1-deltat);
}


float ZuckerMaas::calculateQuality(const std::vector<float> &parameterVector, const std::vector<float> &features){
    //evaluate state
    float score = 0;
    for(unsigned int i = 0; i < features.size(); ++i){
        score += parameterVector[i] * features[i];
    }
    return score;
}

void ZuckerMaas::updateWeights(const State &state, const BorisGoal &action){

}


std::vector<float> ZuckerMaas::getParameterVector(int size){
    if(parameters.empty()){
        //create parameter vector
        parameters.reserve(size);
        for(int i = 0; i<size; ++i){
            parameters.push_back(-1 + (float)rand()/((float)RAND_MAX/(1+1)));
        }
        //debug for a vector that performs reasonably well
        //set weight for numlinesremoved and number of holes
        //parameterVector[22-1] = 1; //linesremoved
        //parameterVector[22-2] = -1; //number of holes
    }
    return parameters;
}
