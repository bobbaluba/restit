#ifndef REINFORCER_H
#define REINFORCER_H

#include <stochyboss.h>

class Reinforcer {
private:
    //StochyBoss greedyPlayer;
    Vector parameterVector;
    Vector momentum;
    double beta; //falloff for momentum
public:
    explicit Reinforcer();
private:
    void updateMomentum();
    void updateWeights();
};

#endif // REINFORCER_H
