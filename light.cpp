#include "light.hpp"

Light::Light(const double x, const double y, const double z, const double _intensity, const Colour& _colour):
	direction(x, y, z), colour(_colour), intensity(_intensity) {
	direction = direction.normalize();
}
