#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <ostream>
#include <istream>

typedef std::vector<double> Vector;
std::ostream& operator << (std::ostream& outs, const Vector& rhs);
std::istream& operator >> (std::istream& ins, Vector& rhs);
const Vector operator- (const Vector& lhs, const Vector& rhs);
const Vector operator- (const Vector& rhs);
const Vector operator*(double lhs, const Vector& rhs);
const Vector operator/(const Vector& lhs, double rhs);
const Vector operator*(const Vector& lhs, double rhs);
const Vector operator+ (const Vector& lhs, const Vector& rhs);
double operator* (Vector& lhs, Vector& rhs);

#endif // VECTOR_H
