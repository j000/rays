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

	Colour colour() const override {
		return _colour;
	}

	double albedo() const override {
		return _albedo;
	}

	double intersects(Ray&) const override;
	Vector surface_normal(const Point&) const override;
};

#endif /* PLANE_HPP */
