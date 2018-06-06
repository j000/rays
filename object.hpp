#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "colour.hpp"
#include "ray.hpp"
#include "vector.hpp"
class Point;

class Object {
public:
	Object(const Colour& c, const double a): _colour(c), _albedo(a) {
	}

	virtual ~Object() {
	}

	virtual Colour colour() const = 0;
	virtual double albedo() const = 0;
	virtual double intersects(const Ray&) const = 0;
	virtual Vector surface_normal(const Point&) const = 0;
protected:
	Colour _colour;
	double _albedo;
};

class Translation: public Object {
public:
	Vector oper;
	Object* child;

	Translation(Object* o, const Vector& v):
		Object(Colour(0, 0, 0), 0), oper(v) {
		child = o;
	}

	Colour colour() const override {
		return child->colour();
	}

	double albedo() const override {
		return child->albedo();
	}

	double intersects(const Ray& ray) const override {
		Ray tmp(ray);
		tmp.origin -= oper;
		return child->intersects(tmp);
	}

	Vector surface_normal(const Point& p) const override {
		Point tmp(p);
		tmp -= oper;
		return child->surface_normal(tmp);
	}
};

#endif /* OBJECT_HPP */
