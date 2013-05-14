#ifndef STOCHYBOSS_H
#define STOCHYBOSS_H

#include "bossofboris.h"

class StochyBoss : public BossOfBoris {
private:
    std::vector<float> parameterVector;
    std::vector<float> delta;
    const float learningRate;
public:
    StochyBoss(float learningRate);
    virtual BorisGoal getGoal(const State &currentState);
    void updateWeights(const std::vector<float> &delta);
private:
    float evaluateAction(BorisGoal action);
    std::vector<int> getFeatures(const State& currentState, const BorisGoal& action);
    std::vector<float> getParameterVector(int size);
    float calculateQuality(const std::vector<float> parameterVector, const std::vector<int> features);

    //from zucker & maas
    float z();
};

#endif // STOCHYBOSS_H
