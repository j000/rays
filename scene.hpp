#ifndef SCENE_HPP
#define SCENE_HPP

#include "sphere.hpp"

#include <cstddef>
#include <vector>

using ::std::size_t;

class Scene
{
public:
	size_t width, height;
	double fov;
	std::vector<Object*> objects;
};

#endif /* SCENE_HPP */
