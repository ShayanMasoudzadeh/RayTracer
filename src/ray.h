#ifndef RAY_H
#define RAY_H

// --------------------------------------
// Class: ray
// Represents a mathematical ray in 3D space,
// defined by an origin point and a direction vector.
// --------------------------------------
class ray {
public:
    // Default constructor (creates an uninitialized ray)
    ray() {}

    // Constructor: creates a ray with a given origin and direction
    ray(const vector3& origin, const vector3& direction)
        : orig(origin), dir(direction) {}

    // Returns the ray's origin point
    const vector3& origin() const { return orig; }

    // Returns the ray's direction vector
    const vector3& direction() const { return dir; }

    // Returns the point along the ray at parameter t:
    // P(t) = origin + t * direction
    vector3 at(double t) const {
        return orig + t * dir;
    }

private:
    vector3 orig; // The starting point of the ray
    vector3 dir;  // The direction vector of the ray
};

#endif // RAY_H