#include "zuckermaas.h"

#include <cassert>
#include <limits>
#include <cmath>
#include <iostream>
#include <iomanip>


//helper methods
double Z(const Vector &theta, const State& x);
const Vector grad_Z(const Vector &theta, const State& x);
const std::vector<SimpleAction> U(const State& x);
double l(const Vector &theta, const State& x, const SimpleAction& u);
const Vector grad_l(const Vector &theta, const State& x, const SimpleAction& u);
double Q(const Vector &theta, const Vector& features);
const Vector grad_Q(const Vector &theta, const Vector &features);
double q(const Vector &theta, const State& x, const SimpleAction& u);
const Vector grad_q(const Vector &theta, const State& x, const SimpleAction &u);
const Vector f(const State& x, const SimpleAction& u);
double r(const State& x, const SimpleAction& u);
const Vector z_tplus1(const Vector &z, double beta, const Vector &theta, const State& xtplus1, const SimpleAction &utplus1);
const Vector delta_tplus1(const Vector &ztplus1, const Vector& deltat, const State& xtplus1, const SimpleAction& utplus1, double t);
const SimpleAction pie_hard(const State& x, const Vector &theta);
const SimpleAction pie_soft(const State& x, const Vector &theta);
const std::pair<SimpleAction,SimpleAction> pie_soft_lookahead(const State& x, const Vector &theta);
const std::pair<SimpleAction, SimpleAction> pie_hard_lookahead(const State &currentState, const Vector &theta);

ZuckerMaas::ZuckerMaas(unsigned int boardFeatures, double learningRate, double momentum):zt(boardFeatures+1, 0), delta(boardFeatures+1, 0), alpha(learningRate), beta(momentum){
    initializeTheta(boardFeatures+1);
}

const SimpleAction ZuckerMaas::getGoal(const State &currentState){
    const bool lookAhead = isUsinglookAhead();
    if(t%1000 == 0){
        std::cout << "t: " << t << std::endl;
        std::cout << "Theta:" << theta << std::endl;
    }
    if(lookAhead){
        const std::pair<SimpleAction,SimpleAction> bestActions(pie_soft_lookahead(currentState, theta));

        //reinforce step
        const SimpleAction &utplus1 = bestActions.second;
        int discard;
        const State xtplus1(currentState.applyAction(bestActions.first, &discard), currentState.getNextPiece(), currentState.getNextPiece());

        zt = z_tplus1(zt,0/*beta for some reason is messing up lookahead*/,theta,xtplus1,utplus1); //not currentState?
        delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

        theta = theta + (alpha * delta);

        ++t;
        return bestActions.first;
    } else {
        const SimpleAction bestAction(pie_soft(currentState, theta));

        //reinforce step
        const SimpleAction &utplus1 = bestAction;
        const State& xtplus1 = currentState;

        zt = z_tplus1(zt,beta,theta,xtplus1,utplus1); //not currentState?
        delta = delta_tplus1(zt, delta, xtplus1, utplus1,t);

        theta = theta + (alpha * delta);

        ++t;
        return bestAction;
    }
}


//Z
double Z(const Vector &theta, const State& x){
    std::vector<SimpleAction> Us = U(x);
    double sum = 0;
    for(unsigned int i=0; i< Us.size(); ++i){
        sum += l(theta, x, Us[i]);
    }
    return sum;
}

//grad Z
const Vector grad_Z(const Vector &theta, const State& x){
    std::vector<SimpleAction> Us = U(x);
    Vector gradZ(theta.size(), 0);
    for(unsigned int j=0; j<Us.size(); ++j){
        const Vector gradlj = grad_l(theta, x, Us[j]);
        for(unsigned int i=0; i<gradlj.size(); ++i){
            gradZ[i] += gradlj[i];
        }
    }
    return gradZ;
}

//U
const std::vector<SimpleAction> U(const State& x){
    return x.getLegalActions();
}

//l
double l(const Vector &theta, const State& x, const SimpleAction& u){
    return exp(Q(theta, f(x, u)));
}

//grad l
const Vector grad_l(const Vector &theta, const State& x, const SimpleAction& u){
    //ugly way of multiplying scalar by a vector
    Vector gradl = grad_Q(theta, f(x, u));
    double scalar = l(theta, x, u);
    for(unsigned int i = 0; i<gradl.size(); ++i){ //TODO use operators instead.
        gradl[i] *= scalar;
    }
    return gradl;
}

//Q
double Q(const Vector &theta, const Vector &features){
    double sum = 0;
    for(unsigned int i = 0; i<features.size(); ++i){
        sum += theta[i] * features[i];
    }
    return sum;
}

//grad Q
const Vector grad_Q(const Vector & /*theta*/, const Vector &features){
    return features;
}

//q
//double q(const Vector theta, const State& x, const SimpleAction& u){
//    double quality = l(theta, x, u)/Z(theta, x);
//    assert(quality>=0);
//    assert(quality<=1);
//    return quality;
//}

//double q(const Vector theta, const State& x, const SimpleAction& u, double ZValue){
//    double quality = l(theta, x, u)/ZValue;
//    if(!(quality>=0) || !(quality<=1)){
//        quality = l(theta, x, u)/Z(theta, x);;
//    }
//    assert(quality>=0);
//    assert(quality<=1);
//    return quality;
//}

double q(const Vector theta, const State& x, const SimpleAction& u, Vector QValues){
    //double quality = l(theta, x, u)/ZValue;
    //if(!(quality>=0) || !(quality<=1)){
    //    quality = l(theta, x, u)/Z(theta, x);;
    //}
    const double QForChosenAction = Q(theta,f(x,u));
    double denominator = 0;
    for(unsigned int i = 0; i < QValues.size(); ++i){
        denominator += exp(QValues[i]-QForChosenAction);
    }
    const double quality = 1/denominator;
    assert(quality>=0);
    assert(quality<=1);
    return quality;
}



//grad q
const Vector grad_q(const Vector &theta, const State& x, const SimpleAction &u){
    return -1.0/double(pow(Z(theta, x),2))*grad_Z(theta, x) * l(theta, x, u) + grad_l(theta, x, u) / Z(theta, x);
}

//f
const Vector f(const State& x, const SimpleAction& u){
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

const Vector computeQValues(const State& x, const Vector& theta, const std::vector<SimpleAction>& Us){
    Vector QValues;
    for(unsigned int i=0; i<Us.size(); ++i){
        const double QValue = Q(theta,f(x,Us[i]));
        QValues.push_back(QValue);
    }
    return QValues;
}

//z_t+1
const Vector z_tplus1(const Vector &z, double beta, const Vector &theta, const State& xtplus1, const SimpleAction &utplus1){
    Vector sum(theta.size(), 0);
    std::vector<SimpleAction> Us = xtplus1.getLegalActions();
    Vector QValues = computeQValues(xtplus1, theta, Us);
    for(unsigned int i = 0; i<Us.size(); ++i){
        sum = sum + grad_Q(theta,f(xtplus1,Us[i])) * q(theta, xtplus1, Us[i], QValues);
    }
    Vector normalizedGradient = grad_Q(theta, f(xtplus1,utplus1)) - sum;
    Vector ret = beta * z + normalizedGradient;
    //Vector ret = beta * z + grad_q(theta, xtplus1, utplus1)/q(theta, xtplus1, utplus1); //floating point error version
    return ret;
}

//delta_t+1
const Vector delta_tplus1(const Vector &ztplus1, const Vector &deltat, const State& xtplus1, const SimpleAction& utplus1, double t){
    const Vector ret = deltat + t/(t+1) * (r(xtplus1, utplus1)*ztplus1-deltat);
    return ret;
}

const SimpleAction pie_soft(const State& x, const Vector &theta){
    std::vector<SimpleAction> Us = x.getLegalActions();
    assert(!Us.empty());
    double actionValue = double(rand())/double(RAND_MAX);
    double actionSum = 0.0;
    const Vector QValues = computeQValues(x,theta,Us);
    for(unsigned int i = 0; i<Us.size(); ++i){
        double quality = q(theta, x, Us[i], QValues);
        actionSum += quality;
        if(actionSum>=actionValue){
            return Us[i];
        }
    }
    //no actions
    assert(false);
}

const std::pair<SimpleAction, SimpleAction> pie_soft_lookahead(const State& x, const Vector &theta){
    std::vector<SimpleAction> Us = x.getLegalActions();
    Vector qualitiesExp;
    qualitiesExp.reserve(Us.size());
    for(unsigned int i = 0; i<Us.size(); ++i){
        //compute Q with lookahead
        int firstLinesRemoved;
        BoardModel nextBoard = x.applyAction(Us[i], &firstLinesRemoved);
        std::vector<SimpleAction> secondActions(nextBoard.getLegalActions(x.getNextPiece()));
        double maxSecondScore = std::numeric_limits<double>::lowest();

        for(unsigned int j=0; j<secondActions.size(); ++j){
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

    if(qualitiesExpSum == 0){
        std::cerr << "Warning: floating point error, falling back to hardmax\n";
        return pie_hard_lookahead(x,theta);
    }

    assert(qualitiesExpSum != 0);
    assert(!Us.empty());

    double actionValue = double(rand())/double(RAND_MAX);
    double actionSum = 0.0;
    for(unsigned int i = 0; i<Us.size(); ++i){
        double probability = qualitiesExp[i]/qualitiesExpSum; //this is softmax
        actionSum += probability;
        if(actionSum>=actionValue){
            int discard;
            State xtplus1(x.applyAction(Us[i], &discard), x.getNextPiece(), x.getNextPiece());
            SimpleAction bestSecondAction = pie_hard(xtplus1, theta);
            return std::pair<SimpleAction,SimpleAction>(Us[i], bestSecondAction);
        }
    }
    //no actions
    assert(false);
}

const SimpleAction pie_hard(const State &x, const Vector &theta){
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

const std::pair<SimpleAction, SimpleAction> pie_hard_lookahead(const State &currentState, const Vector &theta){
    assert(!theta.empty());
    std::vector<SimpleAction> actions = currentState.getLegalActions();
    double maxScore = std::numeric_limits<double>::lowest();
    SimpleAction bestAction = actions[0];
    for(std::vector<SimpleAction>::iterator it = actions.begin(); it!=actions.end(); ++it){
        const SimpleAction& firstAction = *it;

        int numLinesRemoved;
        BoardModel nextBoard = currentState.applyAction(firstAction, &numLinesRemoved);
        std::vector<SimpleAction> secondActions = nextBoard.getLegalActions(currentState.getNextPiece());

        for(unsigned int i = 0; i<secondActions.size(); ++i){
            int numLinesRemovedSecond;
            BoardModel finalBoard = nextBoard.applyAction(secondActions[i], currentState.getNextPiece(), &numLinesRemovedSecond);
            int finalLinesRemoved = numLinesRemovedSecond + numLinesRemoved;

            Vector features = finalBoard.getFeatures();
            features.push_back(finalLinesRemoved);

            double score = Q(theta, features);

            if(score>maxScore){
                maxScore = score;
                bestAction = firstAction;
            }
        }
    }
    int discard;
    const State nextState = State(currentState.applyAction(bestAction, &discard), currentState.getNextPiece(), currentState.getNextPiece());
    const SimpleAction nextAction  = pie_hard(nextState, theta);
    return std::pair<SimpleAction, SimpleAction>(bestAction, nextAction);
}

void ZuckerMaas::initializeTheta(int size){
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
