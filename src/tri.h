#ifndef TRI_H
#define TRI_H

#include "hittable.h"

// ------------------------------------------------------
// Class: tri
// A single triangle object that can be hit by rays.
// Inherits from hittable, so it can be used in any scene.
// Uses barycentric coordinates for hit testing.
// ------------------------------------------------------
class tri : public hittable {
public:
    // ------------------------------------------------------
    // Constructor
    // v0, v1, v2 : vertices of the triangle
    // mat        : material assigned to the triangle
    //
    // Precomputes:
    //   - Plane normal
    //   - Plane equation constant D
    //   - Vector w (used in barycentric coordinate calculation)
    //   - Bounding box
    // ------------------------------------------------------
    tri(const vector3& v0, const vector3& v1, const vector3& v2, shared_ptr<material> mat)
        : v0(v0), v1(v1), v2(v2), mat(mat) 
    {
        // Compute normal using cross product of two edges
        vector3 n = cross(v1 - v0, v2 - v0);
        normal = n.normalize();

        // Plane equation: dot(normal, P) = D
        D = dot(normal, v0);

        // Precompute helper vector for barycentric coordinates
        w = n / dot(n, n);

        // Precompute bounding box
        set_bounding_box();
    }

    // ------------------------------------------------------
    // set_bounding_box()
    // Creates an axis-aligned bounding box that contains
    // all three vertices of the triangle.
    // ------------------------------------------------------
    virtual void set_bounding_box() {
        auto bbox_edge1 = aabb(v0, v1);
        auto bbox_edge2 = aabb(v0, v2);
        bbox = aabb(bbox_edge1, bbox_edge2);
    }

    // Return the triangle's bounding box
    aabb bounding_box() const override { return bbox; }

    // ------------------------------------------------------
    // hit()
    // Checks if a ray intersects the triangle.
    //
    // Steps:
    //  1. Check if the ray is parallel to the plane (dot(normal, dir) ≈ 0).
    //  2. Solve for t (distance along ray to intersection point on plane).
    //  3. Reject if t is outside valid interval.
    //  4. Compute barycentric coordinates (alpha, beta).
    //  5. Reject if point is outside the triangle.
    //  6. Fill hit_record with intersection data.
    // ------------------------------------------------------
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // If denom is near zero, ray is parallel to triangle plane
        if (std::fabs(denom) < 1e-8) return false;

        // Solve for t where ray intersects plane
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t)) return false;

        // Intersection point in 3D space
        auto intersection = r.at(t);

        // Vector from vertex 0 to intersection point
        vector3 planar_hitpt_vector = intersection - v0;

        // Compute barycentric coordinates
        auto alpha = dot(w, cross(planar_hitpt_vector, v2 - v0));
        auto beta  = dot(w, cross(v1 - v0, planar_hitpt_vector));

        // Reject if point is outside triangle
        if ((alpha < 0) || (beta < 0) || (alpha + beta > 1))
            return false;

        // Hit confirmed — store intersection info
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

private:
    vector3 v0, v1, v2;          // Triangle vertices
    shared_ptr<material> mat;    // Material
    aabb bbox;                   // Bounding box
    vector3 normal;              // Unit normal vector
    vector3 w;                   // Precomputed for barycentric coordinate calc
    double D;                    // Plane equation constant
};

#endif // TRI_H
