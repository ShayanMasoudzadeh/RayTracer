#ifndef HITTABLE_H
#define HITTABLE_H

#include "aabb.h"  // For axis-aligned bounding box (used in acceleration structures)

class material;    // Forward declaration to avoid circular include dependency

// ------------------------------------------------------
// Struct: hit_record
// Stores information about a ray-object intersection.
// When a ray hits a surface, the intersection details
// (point, normal, material, etc.) are filled in here.
// ------------------------------------------------------
class hit_record {
public:
    vector3 p;                      // Intersection point in 3D space
    vector3 normal;                 // Surface normal at the hit point
    shared_ptr<material> mat;       // Pointer to the material at the hit point
    double t;                       // Ray parameter at the intersection: P(t) = origin + t * direction
    bool front_face;                // True if the ray hits the front face of the surface

    // --------------------------------------
    // Sets the correct surface normal direction.
    // Ensures the normal always points against
    // the incoming ray direction.
    // 
    // Parameters:
    //  - r: the incoming ray
    //  - outward_normal: the normal pointing outward from the surface
    //    (must be normalized)
    // --------------------------------------
    void set_face_normal(const ray& r, const vector3& outward_normal) {
        // front_face is true if the ray is coming from outside the surface
        front_face = dot(r.direction(), outward_normal) < 0;

        // Ensure normal points against the ray's direction
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// ------------------------------------------------------
// Abstract Base Class: hittable
// Represents any object in the scene that can be hit by a ray.
// Derived classes implement the intersection logic and bounding box.
// ------------------------------------------------------
class hittable {
public:
    virtual ~hittable() = default;

    // Checks if the ray 'r' hits the object between ray_t.min and ray_t.max.
    // If so, fills in 'rec' with hit details and returns true.
    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;

    // Returns the axis-aligned bounding box for this object.
    // Used for BVH acceleration.
    virtual aabb bounding_box() const = 0;
};

#endif // HITTABLE_H
