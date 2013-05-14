#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
public:
    StochyBoss();
    virtual BorisGoal getGoal(const State &currentState);
private:
    float evaluateAction(BorisGoal action);
    std::vector<int> getFeatures(const State& currentState, const BorisGoal& action);
    float calculateQuality(const std::vector<float> parameterVector, const std::vector<int> features);
};

#endif // STOCHYBOSS_H
