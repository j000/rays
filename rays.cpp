#include <iostream>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>

#include "bmpwriter.hpp"

#include "scene.hpp"
#include "sphere.hpp"
#include "plane.hpp"

class Ray;

static constexpr double infinity = std::numeric_limits<double>::infinity();
static constexpr double PI = 3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214;

using std::cout;
using std::clamp;

int main() {
	Scene scene;

	scene.width = 1600;
	scene.height = 1200;
	scene.fov = 75; // vertical

	scene.light.colour = Colour(1., 1., 1.);
	scene.light.intensity = 20.;
	scene.light.direction = Vector(-0.25, -1., -0.6).normalize();

	scene.objects.push_back(new Sphere(0., 0., -3., 1., Colour(0., 1., 0.), 0.18));
	scene.objects.push_back(new Sphere(1., 1., -3., 1.1, Colour(0., 0., 1.), 0.58));
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

				auto shadow_ray = Ray(hit_point, -scene.light.direction);
				auto shadow_dist = scene.trace(shadow_ray).first;

				double intensity = 0.;
				if (shadow_dist == infinity) {
					Vector normal = closest->surface_normal(hit_point);
					double light_power = normal.dot(-scene.light.direction) * scene.light.intensity;
					intensity = clamp(light_power * closest->albedo / PI, 0., 1.);
				}

				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wfloat-conversion"
				bmp[54 + x * 3 + y * (scene.width * 3 + pad)]
						= 0xFF * intensity * closest->colour.blue * scene.light.colour.blue;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 1]
						= 0xFF * intensity * closest->colour.green * scene.light.colour.green;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 2]
						= 0xFF * intensity * closest->colour.red * scene.light.colour.red;
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
