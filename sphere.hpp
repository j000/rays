#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "colour.hpp"
#include "vector.hpp"
#include "object.hpp"

class Sphere: public Object {
public:
	Point center;
	double radius;

	Sphere() = delete;

	Sphere(
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

	double intersects(const Ray&) const override;
	Vector surface_normal(const Point&) const override;
};

#endif /* SPHERE_HPP */
