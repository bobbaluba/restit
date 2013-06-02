#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "vector.h"
#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
private:
    std::vector<double> theta;
public:
    StochyBoss();
    virtual SimpleAction getGoal(const State &currentState);
    void setTheta(const std::vector<double> vector){ theta = vector; }
private:
    double evaluateAction(SimpleAction action);
    std::vector<double> getFeatures(const State& currentState, const SimpleAction& action);
    std::vector<double> createParameterVector(int size);
    double calculateQuality(const Vector &theta, const Vector &features);
};

#endif // STOCHYBOSS_H
