#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vector.hpp"
#include "colour.hpp"

class Light
{
public:
	Vector direction;
	Colour colour;
	double intensity;
};

#endif /* LIGHT_HPP */
