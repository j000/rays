#ifndef LIGHT_HPP
#define LIGHT_HPP

#include "vector.hpp"
#include "colour.hpp"

class Light {
public:
	Vector direction;
	Colour colour;
	double intensity;

	Light() = delete;

	Light(
		const double,
		const double,
		const double,
		const double,
		const Colour&
	);
};

#endif /* LIGHT_HPP */
