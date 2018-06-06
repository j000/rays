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

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
template <typename T>
inline __attribute__((always_inline))
T inv_sqrt(T x) {
	return T(1) / sqrt(x);
}

template <>
inline __attribute__((always_inline))
float inv_sqrt(float x) {
	static_assert(sizeof(float) == sizeof(int), "Sizes must match");

	union {
		float f;
		int i;
	} convert;

	convert.f = x;
	float xhalf = 0.5f * x;
	/* undefined behaviour */
	//auto i = reinterpret_cast<int*>(&x);
	//*i = 0x5f3759df - (*i >> 1);

	convert.i = 0x5f3759df - (convert.i >> 1);
	x = convert.f;

	x = x * (1.5f - xhalf * x * x);
	return x;
}

template <>
inline __attribute__((always_inline))
double inv_sqrt(double x) {
	static_assert(sizeof(double) == sizeof(long int), "Sizes must match");

	union {
		double f;
		long int i;
	} convert;

	convert.f = x;
	double xhalf = 0.5 * x;
	/* undefined behaviour */
	// auto i = reinterpret_cast<long int*>(&x);
	// *i = 0x5fe6eb50c7b537a9 - (*i >> 1);

	convert.i = 0x5fe6eb50c7b537a9 - (convert.i >> 1);
	x = convert.f;

	x = x * (1.5 - xhalf * x * x);
	return x;
}

__attribute__((target_clones("avx,default")))
Vector Vector::normalize() const {
	double length2 = length_squared();

	if (unlikely(length2 <= zero))
		return Vector(0, 0, 0);

	if (unlikely(length2 - 1. <= zero))
		return Vector(*this);

	double norm = inv_sqrt(length2);

	return *this * norm;
}

double Vector::length_squared() const {
	return x * x + y * y + z * z;
}

double Vector::dot(const Vector& v) const {
	return x * v.x + y * v.y + z * v.z;
}

Vector Vector::operator-() const {
	return Vector(-x, -y, -z);
}

Vector& Vector::operator+=(const Vector& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}

////////////////////////////////////////

Point::Point(const double _x, const double _y, const double _z):
	x(_x), y(_y), z(_z) {
}

Point::Point(const Vector& v): x(v.x), y(v.y), z(v.z) {
}

Vector Point::operator-(const Point& rhs) const {
	return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}

Point& Point::operator+=(const Vector& rhs) {
	x += rhs.x;
	y += rhs.y;
	z += rhs.z;
	return *this;
}

Point& Point::operator-=(const Vector& rhs) {
	x -= rhs.x;
	y -= rhs.y;
	z -= rhs.z;
	return *this;
}
