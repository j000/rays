#include "sphere.hpp"
#include "vector.hpp"
#include "ray.hpp"

#include <cmath>

Sphere::Sphere(const double& x, const double& y, const double& z, const double& r, const Colour& c) :
	Object(c), center(x, y, z), radius(r)
{
}

double Sphere::intersects(const Ray& ray) const {
	/* variables names follow Figure 1:
	 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection */

	Vector line = center - ray.origin;
	double tca = line.dot(ray.direction);

	if (tca < 0.) // we are behind sphere
		return INFINITY;

	double dist2 = line.lengthSquared() - tca * tca;
	double radius2 = radius * radius;

	if (dist2 > radius2) // we are not intersecting shpere
		return INFINITY;

	double thc = sqrt(radius2 - dist2);

	double t0 = tca - thc;
	if (t0 > 0.) // otherwise it's behind camera
		return t0;

	double t1 = tca + thc;
	if (t1 > 0.) // is it always true?
		return t1;

	return INFINITY;
}
