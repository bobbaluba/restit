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
BorisGoal pie(const State& x, const std::vector<float> &theta);


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
ZuckerMaas::ZuckerMaas(float alpha):zt(22, 0), delta(22, 0), alpha(alpha), beta(0.5), t(0){
    initializeParameterVector(22);
}

BorisGoal ZuckerMaas::getGoal(const State &currentState){
    BorisGoal bestAction = pie(currentState, theta);

    //reinforce step
    const BorisGoal &utplus1 = bestAction;
    const State& xtplus1 = currentState;

    zt = z_tplus1(zt,beta,theta,xtplus1,utplus1); //not currentState?
    delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

    assert(delta[0]<100000 && delta[0]>-100000);

    theta = theta + (alpha * delta);

    assert(theta[0]<100000 && theta[0]>-100000);

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
    float quality = l(theta, x, u)/Z(theta, x);
    assert(quality>0 && quality<=1);
    return quality;
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
    std::vector<float> ret = beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1);
    if(ret[0]>100000 || ret[0]<-100000){
        beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1);
    }
    return ret;
}

//delta_t+1
std::vector<float> delta_tplus1(const std::vector<float> &ztplus1, std::vector<float> deltat, const State& xtplus1, const BorisGoal& utplus1, float t){
    std::vector<float> ret = deltat + t/(t+1) * (r(xtplus1, utplus1)*ztplus1-deltat);
    return ret;
}

BorisGoal pie(const State& x, const std::vector<float> &theta){
    std::vector<BorisGoal> Us = x.getLegalBorisGoals();
    assert(!Us.empty());
    float actionValue = float(rand())/float(RAND_MAX);
    float actionSum = 0.0f;
    for(unsigned int i = 0; i<Us.size(); ++i){
        float quality = q(theta, x, Us[i]);
        actionSum += quality;
        if(actionSum>=actionValue){
            return Us[i];
        }
    }
    //no actions
    assert(false);
}

void ZuckerMaas::initializeParameterVector(int size){
    if(theta.empty()){
        //create parameter vector
        theta.reserve(size);
        for(int i = 0; i<size; ++i){
            //parameters.push_back(-1 + (float)rand()/((float)RAND_MAX/(1+1)));
            theta.push_back(0);
        }
        //debug for a vector that performs reasonably well
        //set weight for numlinesremoved and number of holes
        theta[22-1] = 1; //linesremoved
        theta[22-2] = -1; //number of holes
    }
}
