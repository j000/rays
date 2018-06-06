#ifndef VECTOR_HPP
#define VECTOR_HPP

#ifndef PRECOMPILED
#include <type_traits>
#endif

class Point;

class Vector {
public:
	double x, y, z;

	Vector(const double = 0., const double = 0., const double = 0.);
	explicit Vector(const Point&);

	Vector normalize() const;
	double length_squared() const;
	double dot(const Vector&) const;

	Vector operator+() const;
	Vector operator-() const;
	Vector& operator+=(const Vector& rhs);
	Vector& operator-=(const Vector& rhs);

	// czarna magia ;) działa tylko dla różnych intów i floatów
	template <typename T,
		std::enable_if_t <std::is_arithmetic_v <T>>* = nullptr>
	Vector& operator*=(const T);
};

class Point {
public:
	double x, y, z;

	Point(const double = 0., const double = 0., const double = 0.);
	explicit Point(const Vector&);

	Vector operator-(const Point&) const;
	Point& operator+=(const Vector&);
	Point& operator-=(const Vector&);
};

////////////////////////////////////////

inline Vector Vector::operator+() const {
	return *this;
}

template <typename T,
	std::enable_if_t <std::is_arithmetic_v <T>>* = nullptr>
Vector& Vector::operator*=(const T rhs) {
	x *= rhs;
	y *= rhs;
	z *= rhs;
	return *this;
}

inline Vector operator+(Vector lhs, const Vector& rhs) {
	lhs += rhs;
	return lhs;
}

inline Vector operator-(Vector lhs, const Vector& rhs) {
	lhs -= rhs;
	return lhs;
}

// czarna magia ;) działa tylko dla różnych intów i floatów
template <typename T,
	std::enable_if_t <std::is_arithmetic_v <T>>* = nullptr>
inline Vector operator*(Vector lhs, const T rhs) {
	lhs *= rhs;
	return lhs;
}

inline Point operator+(Point lhs, const Vector& rhs) {
	lhs += rhs;
	return lhs;
}

inline Point operator+(const Vector& lhs, Point rhs) {
	rhs += lhs;
	return rhs;
}

inline Point operator-(Point lhs, const Vector& rhs) {
	lhs -= rhs;
	return lhs;
}

inline Point operator-(const Vector& lhs, Point rhs) {
	rhs -= lhs;
	return rhs;
}

#endif /* VECTOR_HPP */
