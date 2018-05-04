#ifndef COLOUR_HPP
#define COLOUR_HPP

class Colour
{
public:
	double red, green, blue;

	Colour() = default;
	Colour(const double& r, const double& g, const double& b) :
		red(r), green(g), blue(b) {};
};

#endif /* COLOUR_HPP */
