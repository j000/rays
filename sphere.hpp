#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "colour.hpp"
#include "vector.hpp"
#include "object.hpp"

class Sphere :
	public Object
{
public:
	Vector center;
	double radius;

	Sphere() = default;
	Sphere(const double&, const double&, const double&, const double&, const Colour&);

	double intersects(const Ray&) const override;
};

#endif /* SPHERE_HPP */
