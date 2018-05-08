#ifndef SCENE_HPP
#define SCENE_HPP

#include <cstddef>
#include <vector>

#include "light.hpp"

class Object;

using ::std::size_t;

class Scene
{
public:
	unsigned int width, height;
	double fov;
	std::vector<Object*> objects;
	Light light;
};

#endif /* SCENE_HPP */
