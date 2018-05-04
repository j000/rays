#ifndef OBJECT_HPP
#define OBJECT_HPP

#include "colour.hpp"
class Ray;

class Object
{
public:
	Colour colour;

	Object(const Colour& c): colour(c) {};
	virtual ~Object() {};

	virtual double intersects(const Ray&) const = 0;
};

#endif /* OBJECT_HPP */
