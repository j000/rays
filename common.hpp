#ifndef COMMON_HPP
#define COMMON_HPP

#include <cmath>

static constexpr double infinity = std::numeric_limits<double>::infinity();
// static constexpr double PI = atan(1.) * 4;
// static constexpr double PI = 355. / 113.;
static constexpr double PI =
	3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214;

constexpr double zero = 1e-13;

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#endif /* COMMON_HPP */
