#ifndef ZUCKERMAAS_H
#define ZUCKERMAAS_H

#include "bossofboris.h"

#include <vector>

class ZuckerMaas : public BossOfBoris {
private:
    std::vector<float> theta; //policy parameters
    std::vector<float> zt; //current momentum
    std::vector<float> delta; //current step
    const float alpha; //learning rate 0.2 a good value?
    const float beta; //momemntum importance factor
    int t;
public:
    ZuckerMaas(float alpha);
    virtual BorisGoal getGoal(const State &currentState);
    //void setParameterVector(const std::vector<float> vector){ parameters = vector; }
private:
    float evaluateAction(BorisGoal action);
    std::vector<float> getFeatures(const State& currentState, const BorisGoal& action);
    void initializeParameterVector(int size);
    float calculateQuality(const std::vector<float> &parameterVector, const std::vector<float> &features);
};

#endif // ZUCKERMAAS_H
