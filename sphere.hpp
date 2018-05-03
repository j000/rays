#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "colour.hpp"
#include "vector.hpp"
class Ray;

class Sphere
{
public:
	Vector center;
	double radius;
	Colour colour;

	double intersects(const Ray&);
};

#endif /* SPHERE_HPP */
