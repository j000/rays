#include "plane.hpp"

#include "ray.hpp"

#include <cmath>

Plane::Plane(const double& x1, const double& y1, const double& z1, const double& x2, const double& y2, const double& z2, const Colour& c) :
	Object(c), origin(x1, y1, z1), normal(x2, y2, z2)
{
}

double Plane::intersects(const Ray& ray) const {
	double denom = normal.dot(ray.direction);

	if (denom < 0.)
		return INFINITY;

	Vector line = origin - ray.origin;

	double dist = line.dot(normal) / denom;

	if (dist <= 0.)
		return INFINITY;

	return dist;
}

