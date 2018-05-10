#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstddef>
#include <vector>

#include "light.hpp"
#include "ray.hpp"

class Object;

using ::std::size_t;

class Scene
{
public:
	unsigned int width, height;
	double fov;
	std::vector<Object*> objects;
	Light light;

	Ray create_prime(const unsigned&, const unsigned&);
};

#endif /* SCENE_HPP */
