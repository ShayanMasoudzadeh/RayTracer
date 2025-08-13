#ifndef AABB_H
#define AABB_H

#include "ray_tracer.h"

// ============================================================
// aabb: Axis-Aligned Bounding Box
// 
// Represents a rectangular box in 3D space whose faces are aligned
// with the coordinate axes.
//
// Each axis-aligned bounding box is defined by three 1D intervals
// (x, y, z), one for each axis, storing the min and max extent.
// ============================================================
class aabb {
  public:
    interval x, y, z;  // Ranges of the box along each axis

    // --------------------------------------------------------
    // Default constructor
    // Creates an "empty" AABB because the default interval
    // constructor makes an empty range.
    // --------------------------------------------------------
    aabb() {}

    // --------------------------------------------------------
    // Constructor from explicit x, y, z intervals.
    // Ensures no dimension is too small by calling pad_to_minimums().
    // --------------------------------------------------------
    aabb(const interval& x, const interval& y, const interval& z) : x(x), y(y), z(z) {
        pad_to_minimums();
    }

    // --------------------------------------------------------
    // Constructor from two corner points (a and b).
    // The points may be in any order; this will figure out the
    // correct min and max for each axis.
    // --------------------------------------------------------
    aabb(const vector3& a, const vector3& b) {
        x = (a[0] <= b[0]) ? interval(a[0], b[0]) : interval(b[0], a[0]);
        y = (a[1] <= b[1]) ? interval(a[1], b[1]) : interval(b[1], a[1]);
        z = (a[2] <= b[2]) ? interval(a[2], b[2]) : interval(b[2], a[2]);

        pad_to_minimums();
    }
    
    // --------------------------------------------------------
    // Constructor that merges two AABBs into one that encloses both.
    // Achieved by taking min of mins and max of maxs for each axis.
    // --------------------------------------------------------
    aabb(const aabb& box0, const aabb& box1) {
        x = interval(box0.x, box1.x);
        y = interval(box0.y, box1.y);
        z = interval(box0.z, box1.z);
    }

    // --------------------------------------------------------
    // Returns the interval for a given axis index:
    // axis 0 → x, axis 1 → y, axis 2 → z.
    // --------------------------------------------------------
    const interval& axis_interval(int n) const {
        if (n == 1) return y;
        if (n == 2) return z;
        return x;
    }

    // --------------------------------------------------------
    // Ray-box intersection test.
    // ray_t is the current allowed [t_min, t_max] range of the ray.
    // Returns true if the ray intersects the box.
    //
    // Uses the "slab method":
    // For each axis, compute intersection distances (t0, t1) with
    // the two planes of that axis, then shrink the allowable t range.
    // If the range becomes invalid (t_max <= t_min), no hit occurs.
    // --------------------------------------------------------
    bool hit(const ray& r, interval ray_t) const {
        const vector3& ray_orig = r.origin();
        const vector3& ray_dir  = r.direction();

        for (int axis = 0; axis < 3; axis++) {
            const interval& ax = axis_interval(axis);
            const double adinv = 1.0 / ray_dir[axis]; // Inverse dir component

            auto t0 = (ax.min - ray_orig[axis]) * adinv;
            auto t1 = (ax.max - ray_orig[axis]) * adinv;

            // Ensure t0 <= t1 by swapping if necessary
            if (t0 < t1) {
                if (t0 > ray_t.min) ray_t.min = t0;
                if (t1 < ray_t.max) ray_t.max = t1;
            } else {
                if (t1 > ray_t.min) ray_t.min = t1;
                if (t0 < ray_t.max) ray_t.max = t0;
            }

            // If range is invalid, return false
            if (ray_t.max <= ray_t.min)
                return false;
        }
        return true;
    }

    // --------------------------------------------------------
    // Returns the index (0=x, 1=y, 2=z) of the longest axis
    // in the bounding box, useful for BVH splitting heuristics.
    // --------------------------------------------------------
    int longest_axis() const {
        if (x.size() > y.size())
            return x.size() > z.size() ? 0 : 2;
        else
            return y.size() > z.size() ? 1 : 2;
    }
    
    // Predefined special bounding boxes:
    // - empty: No volume
    // - universe: Infinite volume
    static const aabb empty, universe;

  private:
    // --------------------------------------------------------
    // Ensures no dimension is smaller than a small delta,
    // which avoids degenerate boxes that might break intersection math.
    // --------------------------------------------------------
    void pad_to_minimums() {
        double delta = 0.0001;
        if (x.size() < delta) x = x.expand(delta);
        if (y.size() < delta) y = y.expand(delta);
        if (z.size() < delta) z = z.expand(delta);
    }
};

// Define static members
const aabb aabb::empty    = aabb(interval::empty,    interval::empty,    interval::empty);
const aabb aabb::universe = aabb(interval::universe, interval::universe, interval::universe);

#endif