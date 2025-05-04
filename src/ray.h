#ifndef RAY_H
#define RAY_H

#include "vector3.h"

class ray{
    public:
    ray() {}

    ray(const vector3& origin, const vector3& direction) : orig(origin), dir(direction) {}

    const vector3& origin() const {return orig;}
    const vector3& direction() const {return dir;}

    vector3 at(double t) const {
        return orig + t * dir;
    }

    private:
    vector3 orig;
    vector3 dir;
};

#endif