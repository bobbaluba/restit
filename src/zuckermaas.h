#ifndef ZUCKERMAAS_H
#define ZUCKERMAAS_H

#include "bossofboris.h"

#include <vector>

typedef std::vector<double> Vector;

class ZuckerMaas : public BossOfBoris {
private:
    Vector theta; //policy parameters
    Vector zt; //current momentum
    Vector delta; //current step
    const double alpha; //learning rate 0.2 a good value?
    const double beta; //momemntum importance factor
    int t;
public:
    ZuckerMaas(unsigned int boardFeatures, double learningRate = 0.1, double momentum = 0.5);
    virtual BorisGoal getGoal(const State &currentState);
    //void setParameterVector(const Vector vector){ parameters = vector; }
private:
    double evaluateAction(BorisGoal action);
    void initializeTheta(int size);
    double calculateQuality(const Vector &parameterVector, const Vector &features);
};

#endif // ZUCKERMAAS_H
