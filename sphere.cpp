#include "sphere.hpp"
#include "vector.hpp"
#include "ray.hpp"

bool Sphere::intersects(const Ray& ray) {

	Vector line = center - ray.origin;

	double dist = line.dot(ray.direction);

	double dist2 = line.dot(line) - dist * dist;

	if (dist2 > this->radius * this->radius)
		return false;

	return true;
}
