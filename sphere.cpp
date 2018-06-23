#include "sphere.hpp"
#include "vector.hpp"
#include "ray.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <cmath>
#include <limits>
#endif

Sphere::Sphere(
	const double x,
	const double y,
	const double z,
	const double r,
	const Colour& c,
	const double a
): Object(c, a), center(x, y, z), radius(r) {
}

__attribute__((target_clones("avx,default")))
double Sphere::intersects(Ray& ray) const {
	/* variables names follow Figure 1:
	 * https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection */

	Vector line = center - ray.origin;
	double tca = line.dot(ray.direction);

	if (unlikely(tca < 0.)) // sphere is behind us
		return infinity;

	double dist2 = line.length_squared() - tca * tca;
	double radius2 = radius * radius;

	if (likely(dist2 > radius2)) // we are not intersecting
		return infinity;

	double thc = sqrt(radius2 - dist2);

	double t0 = tca - thc;
	if (likely(t0 > 0.)) // otherwise it's behind camera
		return t0;

	double t1 = tca + thc;
	if (likely(t1 > 0.)) // otherwise it's behind camera
		return t1;

	return infinity;
}

Vector Sphere::surface_normal(const Point& hit_point) const {
	return (hit_point - center).normalize();
}
