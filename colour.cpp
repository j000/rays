#include "colour.hpp"

#ifndef PRECOMPILED
#include <algorithm>
#endif

Colour::Colour(const double r, const double g, const double b) :
	red(std::clamp(r, 0., 1.)), green(std::clamp(g, 0., 1.)), blue(std::clamp(b, 0., 1.))
{
}

Colour Colour::operator* (const double a) const {
	return Colour(std::clamp(red * a, 0., 1.), std::clamp(green * a, 0., 1.), std::clamp(blue * a, 0., 1.));
}

Colour Colour::operator* (const Colour& c) const {
	return Colour(red * c.red, green * c.green, blue * c.blue);
}

Colour operator*(const double a, const Colour& c) {
	return c*a;
}

Colour& Colour::operator+=(const Colour& c) {
	red = std::min(red + c.red, 1.);
	green = std::min(green + c.green, 1.);
	blue = std::min(blue + c.blue, 1.);
	return *this;
}
