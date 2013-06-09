#ifndef ZUCKERMAAS_H
#define ZUCKERMAAS_H

#include "tetrisagent.h"
#include "vector.h"

class ZuckerMaas : public TetrisAgent {
private:
    Vector theta; //policy parameters
    Vector zt; //current momentum
    Vector delta; //current step
    const double alpha; //learning rate 0.2 a good value?
    const double beta; //momemntum importance factor
    long t;
public:
    explicit ZuckerMaas(unsigned int boardFeatures, double learningRate = 0.1, double momentum = 0.0);
    virtual const SimpleAction getGoal(const State &currentState);
    virtual long getTotalMoves() const {return t;}
    //void setParameterVector(const Vector vector){ parameters = vector; }
    const Vector& getTheta() const { return theta; }
    void setTheta(const Vector& newTheta) { theta = newTheta; }
private:
    void initializeTheta(int size);
    double calculateQuality(const Vector &parameterVector, const Vector &features);
};

#endif // ZUCKERMAAS_H
