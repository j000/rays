#ifndef SCENE_HPP
#define SCENE_HPP

#include "ray.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <cstddef>
#include <vector>
#include <cmath>
#include <string>
#include <stdexcept>
#endif

class Object;
class Light;
class Color;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"
template <typename T, typename D = double,
	std::enable_if_t<std::is_floating_point_v<D>>* = nullptr>
class PrecalculatedInverse {
public:
	inline PrecalculatedInverse(T a = T(0)):
		value_inverse(D(1)/a), value(a) {};

	inline T& operator=(const T& rhs) {
		value_inverse = D(1) / rhs;
		return value = rhs;
	}

	inline operator T const&() const {
		return value;
	}

	template <typename F,
		std::enable_if_t <std::is_floating_point_v<F>>* = nullptr>
	friend inline F operator/(F lhs, const PrecalculatedInverse<T>& rhs) {
		return lhs * rhs.value_inverse;
	}

	template <typename F,
		std::enable_if_t <std::is_floating_point_v<F>>* = nullptr>
	friend inline F& operator/=(F& lhs, const PrecalculatedInverse<T>& rhs) {
		return lhs *= rhs.value_inverse;
	}
private:
	D value_inverse;
	T value;
};
#pragma GCC diagnostic pop

class Scene {
public:
	Scene(): width(800), height(600), aspect_ratio(width * 1. / height), antialias(1) {
		set_fov(75.);
	}

	Scene& set_width(const unsigned);
	Scene& set_height(const unsigned);
	Scene& set_antialiasing(const unsigned);
	Scene& set_fov(const double);
	Scene& add_light(Light*);
	Scene& add(Object*);

	void render(const std::string) const;

private:
	std::vector<Object*> objects;
	std::vector<Light*> lights;

	PrecalculatedInverse<unsigned> width, height;
	PrecalculatedInverse<double> aspect_ratio;
	PrecalculatedInverse<unsigned> antialias;
	double fov;

	Ray create_prime(const unsigned, const unsigned, const unsigned, const unsigned) const;
	std::pair<double, Object*> trace(Ray&) const;
};

#endif /* SCENE_HPP */
