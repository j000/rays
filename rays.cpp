#include <iostream>
#include <fstream>
#include <algorithm>

#include "bmpwriter.hpp"

#include "scene.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "sphere.hpp"
#include "plane.hpp"

#include <cmath>

static constexpr double infinity = std::numeric_limits<double>::infinity();
static constexpr double PI = std::atan(1.) * 4;
static_assert(3.141592 < PI);
static_assert(3.141593 > PI);

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
			auto ray = Ray::create_prime(x, y, scene);
			double max_dist = infinity;
			Object* closest = nullptr;

			for (const auto i: scene.objects) {
				double dist = i->intersects(ray);
				if (dist < max_dist) {
					max_dist = dist;
					closest = i;
				}
			}

			if (max_dist != infinity) {
				Vector normal = closest->surface_normal(ray.origin + (ray.direction * max_dist));
				double light_power = normal.dot(-scene.light.direction) * scene.light.intensity;
				double intensity = clamp(light_power * closest->albedo / PI, 0., 1.);

				#pragma GCC diagnostic push
				#pragma GCC diagnostic ignored "-Wfloat-conversion"
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad)]
						= 0xFF * intensity * closest->colour.blue * scene.light.colour.blue;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 1]
						= 0xFF * intensity * closest->colour.green * scene.light.colour.green;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 2]
						= 0xFF * intensity * closest->colour.red * scene.light.colour.red;
				#pragma GCC diagnostic pop
			} else {
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad)] = 0xFF;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 1] = 0xCC;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 2] = 0x99;
			}
		}

	cout << bmp.size() << "\n";

	{
		std::ofstream file_out;
		file_out.open("output.bmp", std::ios::binary | std::ios::out);
		file_out.write(reinterpret_cast<const char*>(bmp.data()), bmp.size());
		file_out.close();
	}
}
