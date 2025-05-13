#ifndef TRI_H
#define TRI_H

#include "hittable.h"

class tri : public hittable {
    public:
    tri(const vector3& v0, const vector3& v1, const vector3& v2, shared_ptr<material> mat) : v0(v0), v1(v1), v2(v2), mat(mat) {
        vector3 n = cross(v1 - v0, v2 - v0);
        normal = n.normalize();
        D = dot(normal, v0);
        w = n / dot(n,n);

        set_bounding_box();
    }

    virtual void set_bounding_box() {
        // Compute the bounding box of all three vertices.
        auto bbox_edge1 = aabb(v0, v1);
        auto bbox_edge2 = aabb(v0, v2);
        bbox = aabb(bbox_edge1, bbox_edge2);
    }

    aabb bounding_box() const override { return bbox; }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < 1e-8) return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t)) return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        vector3 planar_hitpt_vector = intersection - v0;
        auto alpha = dot(w, cross(planar_hitpt_vector, v2));
        auto beta = dot(w, cross(v1, planar_hitpt_vector));
        if ((alpha < 0) || (beta < 0) || (alpha + beta > 1)) return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }
    

    private:
    vector3 v0, v1, v2;
    shared_ptr<material> mat;
    aabb bbox;
    vector3 normal;
    vector3 w;
    double D;
};

#endif