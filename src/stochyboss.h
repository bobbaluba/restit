#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
private:
    std::vector<double> parameterVector;
    std::vector<double> delta;
    const double learningRate;
public:
    StochyBoss(double learningRate);
    virtual BorisGoal getGoal(const State &currentState);
    void setParameterVector(const std::vector<double> vector){ parameterVector = vector; }
private:
    double evaluateAction(BorisGoal action);
    std::vector<double> getFeatures(const State& currentState, const BorisGoal& action);
    std::vector<double> getParameterVector(int size);
    double calculateQuality(const std::vector<double> parameterVector, const std::vector<double> features);
};

#endif // STOCHYBOSS_H
