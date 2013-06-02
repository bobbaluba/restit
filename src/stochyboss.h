#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "vector.h"
#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
private:
    Vector theta;
public:
    explicit StochyBoss();
    virtual const SimpleAction getGoal(const State &currentState);
    void setTheta(const std::vector<double> vector){ theta = vector; }
private:
    double evaluateAction(SimpleAction action);
    Vector getFeatures(const State& currentState, const SimpleAction& action);
    Vector createParameterVector(int size);
    double calculateQuality(const Vector &theta, const Vector &features);
};

#endif // STOCHYBOSS_H
