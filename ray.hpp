#ifndef RAY_HPP
#define RAY_HPP

#include "vector.hpp"
class Scene;

class Ray
{
public:
	Vector origin;
	Vector direction;

	Ray(const Vector& _origin, const Vector& _direction) :
		origin(_origin), direction(_direction)
	{};

	static Ray create_prime(const unsigned& x, const unsigned& y, const Scene& scene);
};

#endif /* RAY_HPP */
