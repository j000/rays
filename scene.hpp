#ifndef SCENE_HPP
#define SCENE_HPP

#include "sphere.hpp"

#include <cstddef>

using ::std::size_t;

class Scene
{
public:
	size_t width, height;
	double fov;
	Sphere sphere;
};

#endif /* SCENE_HPP */
