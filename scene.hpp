#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstddef>
#include <vector>

#include "ray.hpp"

class Object;
class Light;

using ::std::size_t;

class Scene
{
public:
	unsigned width, height;
	double fov;
	std::vector<Object*> objects;
	std::vector<Light*> lights;

	Ray create_prime(const unsigned&, const unsigned&);
	std::pair<double, Object*> trace(const Ray&);
};

#endif /* SCENE_HPP */
