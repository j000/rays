#include "vector.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <cmath>
#endif

Vector::Vector(const double _x, const double _y, const double _z):
	x(_x), y(_y), z(_z) {
}

Vector::Vector(const Point& p): x(p.x), y(p.y), z(p.z) {
}

__attribute__((target_clones("avx,default")))
Vector Vector::normalize() const {
	double length2 = length_squared();

	if (unlikely(length2 <= zero))
		return Vector(0, 0, 0);

	if (unlikely(length2 - 1. <= zero))
		return Vector(*this);

	double norm = 1. / sqrt(length2);
	return Vector(x * norm, y * norm, z * norm);
}

double Vector::lengthSquared() const {
	return dot(*this);
}

double Vector::dot(const Vector& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::operator- () const {
	return Vector(-x, -y, -z);
}

Vector Vector::operator- (const Vector& v) const {
	return Vector(x - v.x, y - v.y, z - v.z);
}

Vector Vector::operator+ (const Vector& v) const {
	return Vector(x + v.x, y + v.y, z + v.z);
}

Vector Vector::operator* (const double a) const {
	return Vector(x * a, y * a, z * a);
}
