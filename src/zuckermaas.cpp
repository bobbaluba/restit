#include "zuckermaas.h"

#include <cassert>
#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>


double Z(const Vector theta, const State& x);
Vector grad_Z(const Vector theta, const State& x);
std::vector<SimpleAction> U(const State& x);
double l(const Vector theta, const State& x, const SimpleAction& u);
Vector grad_l(const Vector theta, const State& x, const SimpleAction& u);
double Q(const Vector theta, Vector features);
Vector grad_Q(const Vector theta, Vector features);
double q(const Vector theta, const State& x, const SimpleAction& u);
Vector grad_q(const Vector &theta, const State& x, const SimpleAction &u);
Vector f(const State& x, const SimpleAction& u);
double r(const State& x, const SimpleAction& u);
Vector z_tplus1(const Vector &z, double beta, const Vector &theta, const State& xtplus1, const SimpleAction &utplus1);
Vector delta_tplus1(const Vector &ztplus1, Vector deltat, const State& xtplus1, const SimpleAction& utplus1, double t);
SimpleAction pie_hard(const State& x, const Vector &theta);
SimpleAction pie_soft(const State& x, const Vector &theta);
SimpleAction pie_soft_lookahead(const State& x, const Vector &theta);

ZuckerMaas::ZuckerMaas(unsigned int boardFeatures, double learningRate, double momentum):zt(boardFeatures+1, 0), delta(boardFeatures+1, 0), alpha(learningRate), beta(momentum), t(0){
    initializeTheta(boardFeatures+1);
}

SimpleAction ZuckerMaas::getGoal(const State &currentState){
    const bool lookAhead = true;
    SimpleAction bestAction;
    if(lookAhead){
        bestAction = pie_soft_lookahead(currentState, theta);
    } else {
        bestAction = pie_soft(currentState, theta);
    }

    //reinforce step
    const SimpleAction &utplus1 = bestAction;
    const State& xtplus1 = currentState;

    zt = z_tplus1(zt,beta,theta,xtplus1,utplus1); //not currentState?
    delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

    theta = theta + (alpha * delta);

    std::cout << theta << std::endl;

    ++t;
    return bestAction;
}


//Z
double Z(const Vector theta, const State& x){
    std::vector<SimpleAction> Us = U(x);
    double sum = 0;
    for(unsigned int i=0; i< Us.size(); ++i){
        sum += l(theta, x, Us[i]);
    }
    return sum;
}

//grad Z
Vector grad_Z(const Vector theta, const State& x){
    std::vector<SimpleAction> Us = U(x);
    Vector gradZ(theta.size(), 0);
    for(unsigned int j=0; j<Us.size(); ++j){
        Vector gradlj = grad_l(theta, x, Us[j]);
        for(unsigned int i=0; i<gradlj.size(); ++i){
            gradZ[i] += gradlj[i];
        }
    }
    return gradZ;
}

//U
std::vector<SimpleAction> U(const State& x){
    return x.getLegalActions();
}

//l
double l(const Vector theta, const State& x, const SimpleAction& u){
    return exp(Q(theta, f(x, u)));
}

//grad l
Vector grad_l(const Vector theta, const State& x, const SimpleAction& u){
    //ugly way of multiplying scalar by a vector
    Vector gradl = grad_Q(theta, f(x, u));
    double scalar = l(theta, x, u);
    for(unsigned int i = 0; i<gradl.size(); ++i){ //TODO use operators instead.
        gradl[i] *= scalar;
    }
    return gradl;
}

//Q
double Q(const Vector theta, Vector features){
    double sum = 0;
    for(unsigned int i = 0; i<features.size(); ++i){
        sum += theta[i] * features[i];
    }
    return sum;
}

//grad Q
Vector grad_Q(const Vector theta, Vector features){
    return features;
}

//q
double q(const Vector theta, const State& x, const SimpleAction& u){
    double quality = l(theta, x, u)/Z(theta, x);
    assert(quality>=0);
    assert(quality<=1);
    return quality;
}


//grad q
Vector grad_q(const Vector &theta, const State& x, const SimpleAction &u){
    return -1.0/double(pow(Z(theta, x),2))*grad_Z(theta, x) * l(theta, x, u) + grad_l(theta, x, u) / Z(theta, x);
}

//f
Vector f(const State& x, const SimpleAction& u){
    //gather features
    int linesRemoved;
    BoardModel nextBoard = x.applyAction(u, &linesRemoved);
    Vector features = nextBoard.getFeatures();
    features.push_back(double(linesRemoved));
    return features;
}

//r - probably not needed
double r(const State& x, const SimpleAction& u){
    int numLinesRemoved;
    x.applyAction(u, &numLinesRemoved);
    return numLinesRemoved;
}

//z_t+1
Vector z_tplus1(const Vector &z, double beta, const Vector &theta, const State& xtplus1, const SimpleAction &utplus1){
    Vector sum(theta.size(), 0);
    std::vector<SimpleAction> Us = xtplus1.getLegalActions();
    for(unsigned int i = 0; i<Us.size(); ++i){
        sum = sum + grad_Q(theta,f(xtplus1,Us[i])) * q(theta, xtplus1, Us[i]);
    }
    Vector normalizedGradient = grad_Q(theta, f(xtplus1,utplus1)) - sum;
    Vector ret = beta * z + normalizedGradient;
    //Vector ret = beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1); //floating point error version
    return ret;
}

//delta_t+1
Vector delta_tplus1(const Vector &ztplus1, Vector deltat, const State& xtplus1, const SimpleAction& utplus1, double t){
    Vector ret = deltat + t/(t+1) * (r(xtplus1, utplus1)*ztplus1-deltat);
    return ret;
}

SimpleAction pie_soft(const State& x, const Vector &theta){
    std::vector<SimpleAction> Us = x.getLegalActions();
    assert(!Us.empty());
    double actionValue = double(rand())/double(RAND_MAX);
    double actionSum = 0.0;
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

SimpleAction pie_soft_lookahead(const State& x, const Vector &theta){
    std::vector<SimpleAction> Us = x.getLegalActions();
    Vector qualitiesExp;
    qualitiesExp.reserve(Us.size());
    for(unsigned int i = 0; i<Us.size(); ++i){
        //compute Q with lookahead
        int firstLinesRemoved;
        BoardModel nextBoard = x.applyAction(Us[i], &firstLinesRemoved);
        std::vector<SimpleAction> secondActions(nextBoard.getLegalActions(x.getNextPiece()));
        double maxSecondScore = std::numeric_limits<double>::lowest();

        for(int j=0; j<secondActions.size(); ++j){
            auto features = f(State(nextBoard, x.getNextPiece(), x.getNextPiece() /* dummy */), secondActions[j]);
            features[features.size()-1] += firstLinesRemoved;
            double secondScore = Q(theta, features);
            if(secondScore > maxSecondScore){
                maxSecondScore = secondScore;
            }
        }
        qualitiesExp.push_back(exp(maxSecondScore));
    }

    double qualitiesExpSum = 0;
    for(unsigned int i = 0; i < qualitiesExp.size(); ++i){
        qualitiesExpSum += qualitiesExp[i];
    }

    assert(!Us.empty());

    double actionValue = double(rand())/double(RAND_MAX);
    double actionSum = 0.0;
    for(unsigned int i = 0; i<Us.size(); ++i){
        double probability = qualitiesExp[i]/qualitiesExpSum; //this is softmax
        actionSum += probability;
        if(actionSum>=actionValue){
            return Us[i];
        }
    }
    //no actions
    assert(false);
}

SimpleAction pie_hard(const State &x, const Vector &theta){
    //hardmax
    std::vector<SimpleAction> Us = x.getLegalActions();
    assert(!Us.empty());
    SimpleAction bestAction = Us[rand()%Us.size()];
    float bestQuality = std::numeric_limits<double>::lowest();
    for(unsigned int i=0; i<Us.size(); ++i){
        float quality = Q(theta, f(x, Us[i]));
        if(quality>bestQuality){
            bestQuality = quality;
            bestAction = Us[i];
        }
    }
    return bestAction;
}

void ZuckerMaas::initializeTheta(int size){
    const bool goodParameters = true;
    if(theta.empty()){
        theta.reserve(size);
        if(goodParameters){
            //create parameter vector
            for(int i = 0; i<size; ++i){
                theta.push_back(0);
            }
            //debug for a vector that performs reasonably well
            //set weight for numlinesremoved and number of holes
            theta[theta.size()-1] = 1; //linesremoved
            theta[theta.size()-2] = -1; //number of holes
        } else {
            //create random parameter vector
            for(int i = 0; i<size; ++i){
                //theta.push_back(-1 + (double)rand()/((double)RAND_MAX/(1+1)));
                theta.push_back(0);
            }
        }
    }
}
