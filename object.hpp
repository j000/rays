#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "colour.hpp"
class Ray;
class Vector;
class Point;

class Object
{
public:
	Colour colour;
	double albedo;

	Object(const Colour& c, const double a): colour(c), albedo(a) {};
	virtual ~Object() {};

	virtual double intersects(const Ray&) const = 0;
	virtual Vector surface_normal(const Point&) const = 0;
};

#endif /* OBJECT_HPP */
