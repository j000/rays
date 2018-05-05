#include "ray.hpp"
#include "scene.hpp"

#include <cmath>

static constexpr double PI = atan(1.) * 4;
static_assert(3.141592 < PI);
static_assert(3.141593 > PI);

/* FIXME: performance - move is to scene to make less calculations */
Ray Ray::create_prime(const unsigned& x, const unsigned& y, const Scene& scene) {
    double sensor_x = (0.5 + x) / scene.width * 2. - 1.;
    double sensor_y = 1. - (0.5 + y) / scene.height * 2.;

    double aspect_ratio = ((double)scene.width) / scene.height;
    sensor_x *= aspect_ratio;

    double fov = tan(PI * 0.5 * scene.fov / 180.);
    sensor_x *= fov;
    sensor_y *= fov;

    return Ray(Vector(), Vector(sensor_x, sensor_y, -1.0).normalize());
}
