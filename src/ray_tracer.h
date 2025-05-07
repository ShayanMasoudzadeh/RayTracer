#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

#include "vector3.h"
#include "interval.h"
#include "color.h"
#include "ray.h"

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#endif