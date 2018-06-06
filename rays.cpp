#include "scene.hpp"
#include "sphere.hpp"
#include "plane.hpp"
#include "light.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#endif

class Ray;

int main() {
	Scene scene;

	scene
		.set_width(1920)
		.set_height(1080)
		.set_antialiasing(2)
		;

	scene.add_light(
		new Light(-.75, -1., -0.6, 3., Colour(1., 1., 1.))
	);
	scene.add_light(
		new Light(.75, -1., -0.6, 3., Colour(1., 1., 1.))
	);

	scene.add(
		new Sphere(0., 0., 3., 1., Colour(0., 1., 0.), 0.18)
	);
	scene.add(
		new Sphere(3., 0., 0., 1., Colour(0., 1., 0.), 0.18)
	);
	scene.add(
		new Sphere(-3., 0., 0., 1., Colour(0., 1., 0.), 0.18)
	);

	scene.add(
		new Sphere(0., 0., -3., 1., Colour(0., 1., 0.), 0.18)
	);
	scene.add(
		new Sphere(1., 1., -3., 1.41, Colour(1., 0., 0.), 0.45)
	);
	scene.add(
		new Plane(0., -2., 0., 0., -1., 0., Colour(.761, .698, .502) * 0.6,
		0.4)
	);

	scene.render("output.bmp");
}
