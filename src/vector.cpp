#include "vector.h"

#include <cassert>
#include <iomanip>

double operator* (Vector& lhs, Vector& rhs){
    assert(lhs.size() == rhs.size());
    double sum = 0;
    for(unsigned int i=0; i<lhs.size(); ++i){
        sum += lhs[i] * rhs[i];
    }
    return sum;
}

const Vector operator+ (const Vector& lhs, const Vector& rhs){
    assert(lhs.size() == rhs.size());
    Vector copy = lhs;
    for(unsigned int i=0; i<rhs.size(); ++i){
        copy[i] += rhs[i];
    }
    return copy;
}

const Vector operator*(const Vector& lhs, double rhs){
    Vector copy = lhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= rhs;
    }
    return copy;
}

const Vector operator/(const Vector& lhs, double rhs){
    return lhs * (1/rhs);
}

const Vector operator*(double lhs, const Vector& rhs){
    return rhs * lhs;
}

const Vector operator- (const Vector& rhs){
    Vector copy = rhs;
    for(unsigned int i=0; i<copy.size(); ++i){
        copy[i] *= -1;
    }
    return copy;
}

const Vector operator- (const Vector& lhs, const Vector& rhs){
    return lhs + (-rhs);
}

std::ostream& operator << (std::ostream& outs, const Vector& rhs){
    outs << std::setprecision(4);
    outs << "[";
    for (Vector::const_iterator ii = rhs.begin(); ii != rhs.end(); ++ii)
    {
        outs << " " << *ii;
    }
    outs << " ]";
    return outs;
    //TODO restore original precision
}

std::istream& operator >> (std::istream& ins, Vector& rhs){
    std::string tmp;
    ins >> tmp; // "["
    ins >> tmp; // "first value"
    while(tmp != "]"){
        rhs.push_back(atof(tmp.c_str()));
        ins >> tmp;
    }
    return ins;
}
