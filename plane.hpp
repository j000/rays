#ifndef PLANE_HPP
#define PLANE_HPP

#include "object.hpp"
#include "vector.hpp"

class Plane: public Object {
public:
	Point origin;
	Vector normal;

	Plane(
		const double,
		const double,
		const double,
		const double,
		const double,
		const double,
		const Colour&,
		const double
	);

	double intersects(const Ray&) const override;
	Vector surface_normal(const Point&) const override;
};

#endif /* PLANE_HPP */
