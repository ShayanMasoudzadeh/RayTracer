#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

// ------------------------------------------------------
// Class: sphere
// A basic 3D sphere object that can be intersected by rays.
// Inherits from hittable, so it can be part of any scene.
// ------------------------------------------------------
class sphere : public hittable {
public:
    // ------------------------------------------------------
    // Constructor
    // center: 3D center position of the sphere
    // radius: sphere radius (negative values are clamped to 0)
    // mat:    material of the sphere
    //
    // Also precomputes the axis-aligned bounding box (AABB).
    // ------------------------------------------------------
    sphere(const vector3& center, double radius, shared_ptr<material> mat)
        : center(center),
          radius(std::fmax(0, radius)), // avoid negative radius
          mat(mat) 
    {
        auto rvec = vector3(radius, radius, radius);
        bbox = aabb(center - rvec, center + rvec); // bounding box from min to max corner
    }

    // ------------------------------------------------------
    // hit()
    // Checks if a ray intersects this sphere within the
    // given t-range (ray_t).
    //
    // Uses the quadratic equation to solve for intersection points:
    // Ray: P(t) = origin + t * direction
    // Sphere: (P - C) • (P - C) = r²
    //
    // Returns true if a hit is found and fills in hit_record.
    // ------------------------------------------------------
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Vector from ray origin to sphere center
        vector3 oc = center - r.origin();

        // Quadratic terms (optimized form)
        auto a = r.direction().length_squared();
        auto h = dot(r.direction(), oc); // half of 'b' in quadratic form
        auto c = oc.length_squared() - radius * radius;

        // Discriminant check
        auto discriminant = h * h - a * c;
        if (discriminant < 0)
            return false; // no intersection

        auto sqrtd = std::sqrt(discriminant);

        // Find the nearest root in the valid range
        auto root = (h - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (h + sqrtd) / a;
            if (!ray_t.surrounds(root))
                return false;
        }

        // Fill hit_record
        rec.t = root;
        rec.p = r.at(rec.t);

        // Outward normal before orientation correction
        vector3 outward_normal = (rec.p - center) / radius;

        rec.mat = mat;
        rec.set_face_normal(r, outward_normal);

        return true;
    }

    // ------------------------------------------------------
    // bounding_box()
    // Returns the precomputed AABB for acceleration.
    // ------------------------------------------------------
    aabb bounding_box() const override { return bbox; }

private:
    vector3 center;                 // Sphere center
    double radius;                  // Sphere radius
    shared_ptr<material> mat;       // Material pointer
    aabb bbox;                      // Precomputed bounding box
};

#endif // SPHERE_H