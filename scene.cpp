#include <cmath>

#include "scene.hpp"

static constexpr double PI = atan(1.) * 4;
static_assert(3.141592 < PI);
static_assert(3.141593 > PI);

Ray Scene::create_prime(const unsigned& x, const unsigned& y) {
    double sensor_x = (0.5 + x) / width * 2. - 1.;
    double sensor_y = 1. - (0.5 + y) / height * 2.;

    double aspect_ratio = 1. * width / height;
    sensor_x *= aspect_ratio;

    double sensor_fov = tan(PI * 0.5 * fov / 180.);
    sensor_x *= sensor_fov;
    sensor_y *= sensor_fov;

    return Ray(Vector(), Vector(sensor_x, sensor_y, -1.0).normalize());
}
