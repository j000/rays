#include "vector.hpp"

#include <cmath>

#define likely(x) __builtin_expect((x),1)
#define unlikely(x) __builtin_expect((x),0)

constexpr double zero = 1e-13;

#ifndef __has_cpp_attribute         // Optional of course.
  #define __has_cpp_attribute(x) 0  // Compatibility with non-clang compilers.
#endif

__attribute__((target_clones("avx,default")))
Vector Vector::normalize() const {
	double length2 = lengthSquared();

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
