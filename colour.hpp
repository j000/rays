#ifndef COLOUR_HPP
#define COLOUR_HPP

class Colour
{
public:
	double red, green, blue;

	Colour() = default;
	Colour(const double, const double, const double);

	Colour operator* (const double) const;
	Colour operator* (const Colour&) const;
	friend Colour operator*(const double, const Colour&);

	Colour& operator+=(const Colour&);
};

#endif /* COLOUR_HPP */
