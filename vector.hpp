#ifndef VECTOR_HPP
#define VECTOR_HPP

class Vector
{
public:
	double x, y, z;

	Vector() :
		x(), y(), z()
	{};
	Vector(const double& _x, const double& _y, const double& _z) :
		x(_x), y(_y), z(_z)
	{};
	Vector(const double& _x) :
		x(_x), y(_x), z(_x)
	{};

	Vector normalize() const;
	double lengthSquared() const;
	double dot(const Vector&) const;

	Vector operator- () const;
	Vector operator- (const Vector&) const;
	Vector operator+ (const Vector&) const;
	Vector operator* (const double) const;
};

#endif /* VECTOR_HPP */
