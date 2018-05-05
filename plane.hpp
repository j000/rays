#ifndef PLANE_HPP
#define PLANE_HPP

#include "object.hpp"
#include "vector.hpp"

class Plane :
	public Object
{
public:
	Vector origin;
	Vector normal;

	Plane(const double, const double, const double, const double, const double, const double, const Colour&);

	double intersects(const Ray&) const override;
};

#endif /* PLANE_HPP */
