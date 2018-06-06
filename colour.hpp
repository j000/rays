#ifndef COLOUR_HPP
#define COLOUR_HPP

class Colour {
public:
	double red, green, blue;

	Colour(const double = 0., const double = 0., const double = 0.);

	bool operator==(const Colour& rhs);

	inline bool operator!=(const Colour& rhs) {
		return !(*this == rhs);
	}

	Colour& operator+=(const Colour&);
	friend Colour operator+(Colour, const Colour&);

	Colour& operator*=(const double);
	friend Colour operator*(Colour, const double);
	friend Colour operator*(const double, Colour);

	Colour operator*(const Colour&) const;
};

inline Colour operator+(Colour lhs, const Colour& rhs) {
	return lhs += rhs;
}

inline Colour operator*(Colour lhs, const double rhs) {
	return lhs *= rhs;
}

inline Colour operator*(const double lhs, Colour rhs) {
	return rhs *= lhs;
}

#endif /* COLOUR_HPP */
