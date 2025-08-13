#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>

using std::shared_ptr;
using std::make_shared;

// Common constants for ray tracing
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Converts degrees to radians
inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1)
inline double random_double() {
    return std::rand() / (RAND_MAX + 1.0);
}

// Returns a random real in [min,max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Returns a random integer in [min,max]
inline int random_int(int min, int max) {
    return int(random_double(min, max+1));
}

// Include core ray tracer components
#include "vector3.h"
#include "interval.h"
#include "color.h"
#include "ray.h"

#endif
