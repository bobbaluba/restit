#ifndef REINFORCER_H
#define REINFORCER_H

#include <stochyboss.h>

class Reinforcer {
private:
    //StochyBoss greedyPlayer;
    std::vector<float> parameterVector;
    std::vector<float> momentum;
    float beta; //falloff for momentum
public:
    Reinforcer();
private:
    void updateMomentum();
    void updateWeights();
};

#endif // REINFORCER_H
