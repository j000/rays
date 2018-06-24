#include "scene.hpp"
#include "object.hpp"
#include "light.hpp"
#include "bmpwriter.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <cmath>
#include <algorithm>
#include <tuple>
#include <utility>
#include <iostream>
#include <fstream>
#endif

using std::pair,
	std::make_pair,
	std::cout,
	std::clamp,
	std::invalid_argument;

Scene& Scene::set_width(const unsigned w) {
	if (w == 0)
		throw invalid_argument("Width cannot be zero");
	width = w;
	return *this;
}

Scene& Scene::set_height(const unsigned h) {
	if (h == 0)
		throw invalid_argument("Height cannot be zero");
	height = h;
	return *this;
}

Scene& Scene::set_antialiasing(const unsigned aa) {
	if (aa == 0 || (aa & (aa - 1)))
		throw invalid_argument("Number of samples needs to be a power of two");
	antialias = aa;
	return *this;
}

Scene& Scene::set_fov(const double angle) {
	if (angle <= 0. || angle > 180.)
		throw invalid_argument("FOV angle needs to be in range (0, 180]");
	fov = PI / 180. * angle * 0.5;
	return *this;
}

Scene& Scene::add_light(Light* light) {
	lights.push_back(light);
	return *this;
}

Scene& Scene::add(Object* stuff) {
	objects.push_back(stuff);
	return *this;
}

__attribute__((target_clones("avx,default")))
__attribute__((flatten))
void Scene::render(const std::string filename) const {
	auto bmp = create_bitmap(nullptr, width, height);

	const uint8_t pad = (4 - (width & 3)) & 3;

	#pragma omp parallel for collapse(2)
	for (auto y = 0u; y < height; ++y)
		for (auto x = 0u; x < width; ++x) {
			Colour colour(0, 0, 0);
			for (auto y2 = 0u; y2 < antialias; ++y2)
				for (auto x2 = 0u; x2 < antialias; ++x2) {
					double max_dist = infinity;
					Object* closest = nullptr;
					auto ray = create_prime(x, y, x2, y2);

					std::tie(max_dist, closest) = trace(ray);

					if (max_dist == infinity) {
						colour
							+= 1. / antialias / antialias
							* Colour(.6, .6, 1.);
						continue;
					}

					auto hit_point = ray.origin
						+ (ray.direction * max_dist);

					Colour tmp(0, 0, 0);

					for (const auto light: lights) {
						auto shadow_ray
							= Ray(hit_point, -light->direction);
						auto shadow_dist
							= trace(shadow_ray).first;

						if (shadow_dist == infinity) {
							Vector normal
								= closest->surface_normal(hit_point);
							double light_power
								= normal.dot(-light->direction)
								* light->intensity;
							double intensity = clamp(
									light_power * closest->albedo() / PI,
									0.,
									1.);
							tmp += intensity
								* closest->colour() * light->colour;
						}

						colour += 1. / antialias / antialias * tmp;
					}

					// TODO: ambient light
					colour += .1 / antialias / antialias * closest->colour();
				}
			bmp[54 + x * 3 + y * (width * 3 + pad)]
				= static_cast<uint8_t>(0xFF * colour.blue);
			bmp[54 + x * 3 + y * (width * 3 + pad) + 1]
				= static_cast<uint8_t>(0xFF * colour.green);
			bmp[54 + x * 3 + y * (width * 3 + pad) + 2]
				= static_cast<uint8_t>(0xFF * colour.red);
		}

	cout << bmp.size() << "\n";

	{
		std::ofstream file_out;
		file_out.open(filename, std::ios::binary | std::ios::out);
		file_out.write(
			reinterpret_cast<const char*>(bmp.data()),
			static_cast<long>(bmp.size())
		);
		file_out.close();
	}
}

Ray Scene::create_prime(
		const unsigned x, const unsigned y,
		const unsigned aa_x, const unsigned aa_y
	) const {

	// x mapped to [-1, 1]
	double sensor_x = ((0.5 + aa_x) / antialias + x) / width * 2. - 1.;
	// y mapped to [-1, 1]
	double sensor_y = ((0.5 + aa_y) / antialias + y) / height * 2. - 1.;

	double aspect = 1. * width / height;
	if (aspect > 1.)
		sensor_y /= aspect; // y depends on aspect ratio
	else
		sensor_x *= aspect; // x depends on aspect ratio

	// fisheye
	if (fov > PI / 4) {
		sensor_x *= fov;
		sensor_y *= fov;

		return Ray(Point(), Vector(
			// -sin(sensor_y + PI / 2) * cos(sensor_x + PI / 2),
			cos(sensor_y) * sin(sensor_x),
			// -cos(sensor_y + PI / 2),
			sin(sensor_y),
			// -sin(sensor_y + PI / 2) * sin(sensor_x + PI / 2)
			-cos(sensor_y) * cos(sensor_x)
		));
	}

	// rectilinear
	sensor_x *= tan(fov);
	sensor_y *= tan(fov);

	return Ray(Point(), Vector(sensor_x, sensor_y, -1.).normalize());
}

pair<double, Object*> Scene::trace(Ray& ray) const {
	double max_dist = infinity;
	Object* closest = nullptr;

	for (const auto i: objects) {
		double dist = i->intersects(ray);
		if (dist < max_dist) {
			max_dist = dist;
			closest = i;
		}
	}

	return make_pair(max_dist, closest);
}
