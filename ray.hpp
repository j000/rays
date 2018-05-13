#ifndef RAY_HPP
#define RAY_HPP

#include "vector.hpp"

class Ray
{
public:
	Vector origin;
	Vector direction;

	Ray() = delete;
	Ray(const Vector& _origin, const Vector& _direction) :
		origin(_origin), direction(_direction)
	{};
};

#endif /* RAY_HPP */
