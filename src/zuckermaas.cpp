#include "zuckermaas.h"

#include <cassert>
#include <limits>
#include <cmath>


double Z(const std::vector<double> theta, const State& x);
std::vector<double> grad_Z(const std::vector<double> theta, const State& x);
std::vector<BorisGoal> U(const State& x);
double l(const std::vector<double> theta, const State& x, const BorisGoal& u);
std::vector<double> grad_l(const std::vector<double> theta, const State& x, const BorisGoal& u);
double Q(const std::vector<double> theta, std::vector<double> features);
std::vector<double> grad_Q(const std::vector<double> theta, std::vector<double> features);
double q(const std::vector<double> theta, const State& x, const BorisGoal& u);
std::vector<double> grad_q(const std::vector<double> &theta, const State& x, const BorisGoal &u);
std::vector<double> f(const State& x, const BorisGoal& u);
double r(const State& x, const BorisGoal& u);
std::vector<double> z_tplus1(const std::vector<double> &z, double beta, const std::vector<double> &theta, const State& xtplus1, const BorisGoal &utplus1);
std::vector<double> delta_tplus1(const std::vector<double> &ztplus1, std::vector<double> deltat, const State& xtplus1, const BorisGoal& utplus1, double t);
BorisGoal pie_hard(const State& x, const std::vector<double> &theta);
BorisGoal pie_soft(const State& x, const std::vector<double> &theta);


double operator* (std::vector<double>& lhs, std::vector<double>& rhs){
    assert(lhs.size() == rhs.size());
    double sum = 0;
    for(unsigned int i=0; i<lhs.size(); ++i){
        sum += lhs[i] * rhs[i];
    }
    return sum;
}

std::vector<double> operator+ (const std::vector<double>& lhs, const std::vector<double>& rhs){
    assert(lhs.size() == rhs.size());
    std::vector<double> copy = lhs;
    for(unsigned int i=0; i<rhs.size(); ++i){
        copy[i] += rhs[i];
    }
    return copy;
}

std::vector<double> operator*(const std::vector<double>& lhs, double rhs){
    std::vector<double> copy = lhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= rhs;
    }
    return copy;
}

std::vector<double> operator/(const std::vector<double>& lhs, double rhs){
    return lhs * (1/rhs);
}

std::vector<double> operator*(double lhs, const std::vector<double>& rhs){
    return rhs * lhs;
}

std::vector<double> operator- (const std::vector<double>& rhs){
    std::vector<double> copy = rhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= -1;
    }
    return copy;
}

std::vector<double> operator- (const std::vector<double>& lhs, const std::vector<double>& rhs){
    return lhs + (-rhs);
}
ZuckerMaas::ZuckerMaas(double alpha):zt(22, 0), delta(22, 0), alpha(0.2), beta(0.5), t(0){
    initializeParameterVector(22);
}

BorisGoal ZuckerMaas::getGoal(const State &currentState){
    BorisGoal bestAction;
    const bool softmax = true;
    if(softmax){
        bestAction = pie_soft(currentState, theta);
    } else {
        bestAction = pie_hard(currentState, theta);
    }

    //reinforce step
    const BorisGoal &utplus1 = bestAction;
    const State& xtplus1 = currentState;

    zt = z_tplus1(zt,beta,theta,xtplus1,utplus1); //not currentState?
    delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

    theta = theta + (alpha * delta);

    ++t;
    return bestAction;
}

//Z
double Z(const std::vector<double> theta, const State& x){
    std::vector<BorisGoal> Us = U(x);
    double sum = 0;
    for(unsigned int i=0; i< Us.size(); ++i){
        sum += l(theta, x, Us[i]);
    }
    return sum;
}

//grad Z
std::vector<double> grad_Z(const std::vector<double> theta, const State& x){
    std::vector<BorisGoal> Us = U(x);
    std::vector<double> gradZ(22, 0);
    for(unsigned int j=0; j<Us.size(); ++j){
        std::vector<double> gradlj = grad_l(theta, x, Us[j]);
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
double l(const std::vector<double> theta, const State& x, const BorisGoal& u){
    return exp(Q(theta, f(x, u)));
}

//grad l
std::vector<double> grad_l(const std::vector<double> theta, const State& x, const BorisGoal& u){
    //ugly way of multiplying scalar by a vector
    std::vector<double> gradl = grad_Q(theta, f(x, u));
    double scalar = l(theta, x, u);
    for(unsigned int i = 0; i<gradl.size(); ++i){
        gradl[i] *= scalar;
    }
    return gradl;
}

//Q
double Q(const std::vector<double> theta, std::vector<double> features){
    double sum = 0;
    for(unsigned int i = 0; i<features.size(); ++i){
        sum += theta[i] * features[i];
    }
    return sum;
}

//grad Q
std::vector<double> grad_Q(const std::vector<double> theta, std::vector<double> features){
    return features;
}

//q
double q(const std::vector<double> theta, const State& x, const BorisGoal& u){
    double quality = l(theta, x, u)/Z(theta, x);
    assert(quality>0 && quality<=1);
    return quality;
}


//grad q
std::vector<double> grad_q(const std::vector<double> &theta, const State& x, const BorisGoal &u){
    return -1.0f/double(pow(Z(theta, x),2))*grad_Z(theta, x) * l(theta, x, u) + grad_l(theta, x, u) / Z(theta, x);
}

//f
std::vector<double> f(const State& x, const BorisGoal& u){
    //gather features
    int linesRemoved;
    BoardModel nextBoard = x.applyAction(u, &linesRemoved);
    std::vector<double> features = nextBoard.getFeatures();
    features.push_back(double(linesRemoved));
    return features;
}

//r - probably not needed
double r(const State& x, const BorisGoal& u){
    int numLinesRemoved;
    x.applyAction(u, &numLinesRemoved);
    return numLinesRemoved;
}

//z_t+1
std::vector<double> z_tplus1(const std::vector<double> &z, double beta, const std::vector<double> &theta, const State& xtplus1, const BorisGoal &utplus1){
    std::vector<double> ret = beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1);
    //if(ret[0]>100000 || ret[0]<-100000){
    //    beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1);
    //}
    return ret;
}

//delta_t+1
std::vector<double> delta_tplus1(const std::vector<double> &ztplus1, std::vector<double> deltat, const State& xtplus1, const BorisGoal& utplus1, double t){
    std::vector<double> ret = deltat + t/(t+1) * (r(xtplus1, utplus1)*ztplus1-deltat);
    return ret;
}

BorisGoal pie_soft(const State& x, const std::vector<double> &theta){
    std::vector<BorisGoal> Us = x.getLegalBorisGoals();
    assert(!Us.empty());
    double actionValue = double(rand())/double(RAND_MAX);
    double actionSum = 0.0f;
    for(unsigned int i = 0; i<Us.size(); ++i){
        double quality = q(theta, x, Us[i]);
        actionSum += quality;
        if(actionSum>=actionValue){
            return Us[i];
        }
    }
    //no actions
    assert(false);
}

BorisGoal pie_hard(const State &x, const std::vector<double> &theta){
    //hardmax
    std::vector<BorisGoal> Us = x.getLegalBorisGoals();
    assert(!Us.empty());
    BorisGoal bestAction = Us[rand()%Us.size()];
    float bestQuality = std::numeric_limits<double>::lowest();
    for(int i=0; i<Us.size(); ++i){
        float quality = Q(theta, f(x, Us[i]));
        if(quality>bestQuality){
            bestQuality = quality;
            bestAction = Us[i];
        }
    }
    return bestAction;
}

void ZuckerMaas::initializeParameterVector(int size){
    const bool goodParameters = false;
    if(theta.empty()){
        theta.reserve(size);
        if(goodParameters){
            //create parameter vector
            for(int i = 0; i<size; ++i){
                theta.push_back(0);
            }
            //debug for a vector that performs reasonably well
            //set weight for numlinesremoved and number of holes
            theta[22-1] = 1; //linesremoved
            theta[22-2] = -1; //number of holes
        } else {
            //create random parameter vector
            for(int i = 0; i<size; ++i){
                theta.push_back(-1 + (double)rand()/((double)RAND_MAX/(1+1)));
            }
        }
    }
}
