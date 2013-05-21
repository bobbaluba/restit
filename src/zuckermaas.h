#ifndef ZUCKERMAAS_H
#define ZUCKERMAAS_H

#include "bossofboris.h"

#include <vector>

class ZuckerMaas : public BossOfBoris {
private:
    std::vector<double> theta; //policy parameters
    std::vector<double> zt; //current momentum
    std::vector<double> delta; //current step
    const double alpha; //learning rate 0.2 a good value?
    const double beta; //momemntum importance factor
    int t;
public:
    ZuckerMaas(double alpha);
    virtual BorisGoal getGoal(const State &currentState);
    //void setParameterVector(const std::vector<double> vector){ parameters = vector; }
private:
    double evaluateAction(BorisGoal action);
    std::vector<double> getFeatures(const State& currentState, const BorisGoal& action);
    void initializeParameterVector(int size);
    double calculateQuality(const std::vector<double> &parameterVector, const std::vector<double> &features);
};

#endif // ZUCKERMAAS_H
