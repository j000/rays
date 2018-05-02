#include <iostream>
#include <fstream>

#include "bmpwriter.hpp"

#include "scene.hpp"
#include "ray.hpp"

#include <cmath>

using std::cout;

int main() {
	Scene scene = {0};

	scene.width = 1600;
	scene.height = 1200;
	scene.fov = 90; // vertical

	scene.sphere.center.x = 0.;
	scene.sphere.center.y = 0.;
	scene.sphere.center.z = -sqrt(2.);

	scene.sphere.radius = 1.;

	scene.sphere.colour.red = 0.;
	scene.sphere.colour.green = 1.;
	scene.sphere.colour.blue = 0.;

	auto bmp = create_bitmap(nullptr, scene.width, scene.height);

	uint8_t pad = (4 - (scene.width & 3)) & 3;

	for (auto y = 0u; y < scene.height; ++y)
		for (auto x = 0u; x < scene.width; ++x) {
			auto ray = Ray::create_prime(x, y, scene);

			if (scene.sphere.intersects(ray)) {
				bmp[54 + x * 3 + y * (scene.width * 3 + pad)] = 0xFF * scene.sphere.colour.blue;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 1] = 0xFF * scene.sphere.colour.green;
				bmp[54 + x * 3 + y * (scene.width * 3 + pad) + 2] = 0xFF * scene.sphere.colour.red;
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
