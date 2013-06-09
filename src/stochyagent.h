#ifndef STOCHYAGENT_H
#define STOCHYAGENT_H

#include "vector.h"
#include "tetrisagent.h"

class StochyAgent : public TetrisAgent {
private:
    Vector theta;
public:
    explicit StochyAgent();
    virtual const SimpleAction getGoal(const State &currentState);
    void setTheta(const std::vector<double> &vector){ theta = vector; }
private:
    double evaluateAction(SimpleAction action);
    Vector getFeatures(const State& currentState, const SimpleAction& action);
    Vector createParameterVector(int size);
    double calculateQuality(const Vector &theta, const Vector &features);
};

#endif // STOCHYAGENT_H
