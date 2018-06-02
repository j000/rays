#ifndef SCENE_HPP
#define SCENE_HPP

#include "ray.hpp"

#ifndef PRECOMPILED
#include <cstddef>
#include <vector>
#endif

class Object;
class Light;

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
