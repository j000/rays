#include "bmpwriter.hpp"

#include "scene.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "light.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>
#endif

class Ray;


using std::cout, std::clamp;

int main() {
	Scene scene;

	scene.width = 1600;
	scene.height = 1200;
	scene.fov = 75; // vertical

	scene.lights.push_back(new Light(-.75, -1., -0.6, 20., Colour(1., 1., 1.)));
	scene.lights.push_back(new Light(.75, -1., -0.6, 5., Colour(1., .9, .5)));

	scene.objects.push_back(new Sphere(0., 0., -3., 1., Colour(0., 1., 0.), 0.18));
	scene.objects.push_back(new Sphere(1., 1., -3., 1.41, Colour(0., 0., 1.), 0.45));
	scene.objects.push_back(new Plane(0., -2., 0., 0., -1., 0., Colour(.5, .25, .25), 0.13));

	auto bmp = create_bitmap(nullptr, scene.width, scene.height);

	uint8_t pad = (4 - (scene.width & 3)) & 3;

	for (auto y = 0u; y < scene.height; ++y)
		for (auto x = 0u; x < scene.width; ++x) {
			auto ray = scene.create_prime(x, y);
			double max_dist = infinity;
			Object* closest = nullptr;

			std::tie(max_dist, closest) = scene.trace(ray);

			if (max_dist != infinity) {
				auto hit_point = ray.origin + (ray.direction * max_dist);

				Colour colour(0, 0, 0);

				for (const auto light: scene.lights) {
					auto shadow_ray = Ray(hit_point, -light->direction);
					auto shadow_dist = scene.trace(shadow_ray).first;

					if (shadow_dist == infinity) {
						Vector normal = closest->surface_normal(hit_point);
						double light_power = normal.dot(-light->direction) * light->intensity;
						double intensity = clamp(light_power * closest->albedo / PI, 0., 1.);
						colour += intensity * closest->colour * light->colour;
					}
				}

				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wfloat-conversion"
				bmp[54 + x * 3 + y * (scene.width * 3 + pad)]
						= 0xFF * colour.blue;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 1]
						= 0xFF * colour.green;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 2]
						= 0xFF * colour.red;
				#pragma GCC diagnostic pop
			} else {
				bmp[54 + x * 3 + y * (scene.width * 3 + pad)] = 0xFF;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 1] = 0xCC;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 2] = 0x99;
			}
		}

	cout << bmp.size() << "\n";

	{
		std::ofstream file_out;
		file_out.open("output.bmp", std::ios::binary | std::ios::out);
		file_out.write(reinterpret_cast<const char*>(bmp.data()), static_cast<long>(bmp.size()));
		file_out.close();
	}
}
