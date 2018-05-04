#include <iostream>
#include <fstream>

#include "bmpwriter.hpp"

#include "scene.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "sphere.hpp"

#include <cmath>

using std::cout;

int main() {
	Scene scene;

	scene.width = 1600;
	scene.height = 1200;
	scene.fov = 90; // vertical

	scene.objects.push_back(new Sphere(0., 0., -sqrt(2.), 1., Colour(0., 1., 0.)));
	scene.objects.push_back(new Sphere(1., 0., -sqrt(2.), 1., Colour(0., 0., 1.)));

	auto bmp = create_bitmap(nullptr, scene.width, scene.height);

	uint8_t pad = (4 - (scene.width & 3)) & 3;

	for (auto y = 0u; y < scene.height; ++y)
		for (auto x = 0u; x < scene.width; ++x) {
			auto ray = Ray::create_prime(x, y, scene);
			double max_dist = INFINITY;
			Object* closest = nullptr;

			for (const auto i: scene.objects) {
				double dist = i->intersects(ray);
				if (dist < max_dist) {
					max_dist = dist;
					closest = i;
				}
			}

			if (max_dist != INFINITY) {
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad)] = 0xFF * closest->colour.blue;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 1] = 0xFF * closest->colour.green;
				bmp[54 + x * 3 + (scene.height - 1 - y) * (scene.width * 3 + pad) + 2] = 0xFF * closest->colour.red;
			}
		}

	cout << bmp.size() << "\n";

	{
		std::ofstream file_out;
		file_out.open("output.bmp");
		file_out.write((const char*)bmp.data(), bmp.size());
		file_out.close();
	}
}
