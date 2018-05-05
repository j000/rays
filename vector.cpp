#include "vector.hpp"

#include <cmath>

__attribute__((target_clones("avx2","sse4","sse2","default")))
Vector Vector::normalize() const {
	double length2 = lengthSquared();
	if (length2 <= 0)
		return Vector();
	double norm = 1. / sqrt(length2);
	return Vector(x * norm, y * norm, z * norm);
}

double Vector::lengthSquared() const {
	return dot(*this);
}

double Vector::dot(const Vector& v) const {
	return x * v.x + y * v.y + z * v.z;
}

double Vector::operator* (const Vector& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::operator- (const Vector& v) const {
	return Vector(x - v.x, y - v.y, z - v.z);
}
