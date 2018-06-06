#include "colour.hpp"

#ifndef PRECOMPILED
#include <algorithm>
#endif

using std::clamp, std::swap, std::min;

Colour::Colour(const double r, const double g, const double b):
		red(clamp(r, 0., 1.)),
		green(clamp(g, 0., 1.)),
		blue(clamp(b, 0., 1.)) {
}

bool Colour::operator==(const Colour& rhs) {
	if (red == rhs.red && green == rhs.green && blue == rhs.blue)
		return true;
	return false;
}

Colour& Colour::operator+=(const Colour& rhs) {
	red = min(red + rhs.red, 1.);
	green = min(green + rhs.green, 1.);
	blue = min(blue + rhs.blue, 1.);
	return *this;
}

Colour& Colour::operator*=(const double rhs) {
	red *= rhs;
	green *= rhs;
	blue *= rhs;
	return *this;
}

Colour Colour::operator*(const Colour& rhs) const {
	return Colour(red * rhs.red, green * rhs.green, blue * rhs.blue);
}
