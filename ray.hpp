#ifndef RAY_HPP
#define RAY_HPP

#include "vector.hpp"

class Ray
{
public:
	Point origin;
	Vector direction;

	Ray() = delete;

	Ray(const Point& _origin, const Vector& _direction):
		origin(_origin), direction(_direction) {
	}
};

#endif /* RAY_HPP */
