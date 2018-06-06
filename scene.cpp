#include "scene.hpp"
#include "object.hpp"

#ifndef PRECOMPILED
#include "common.hpp"
#include <cmath>
#include <utility>
#endif

using std::pair, std::make_pair;


Ray Scene::create_prime(const unsigned& x, const unsigned& y) {
    double sensor_x = (0.5 + x) / width * 2. - 1.;
    double sensor_y = (0.5 + y) / height * 2. - 1.;

    double aspect_ratio = 1. * width / height;
    sensor_x *= aspect_ratio;

    double sensor_fov = tan(PI * 0.5 * fov / 180.);
    sensor_x *= sensor_fov;
    sensor_y *= sensor_fov;

	return Ray(Point(), Vector(sensor_x, sensor_y, -1.).normalize());
}

std::pair<double, Object*> Scene::trace(const Ray& ray) {
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
