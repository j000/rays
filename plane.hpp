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

	Plane(const double, const double, const double, const double, const double, const double, const Colour&, const double);

	double intersects(const Ray&) const override;
	Vector surface_normal(const Vector&) const override;
};

#endif /* PLANE_HPP */
