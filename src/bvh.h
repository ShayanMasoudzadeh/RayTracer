#ifndef BVH_H
#define BVH_H

#include "aabb.h"
#include "hittable.h"
#include "hittable_list.h"

#include <algorithm>

// ============================================================
// bvh_node: Bounding Volume Hierarchy node
//
// This class is a spatial acceleration structure for ray tracing.
// It organizes hittable objects into a binary tree where each node
// has a bounding box that contains all of its child objects.
//
// The BVH drastically reduces ray-object intersection tests by
// quickly skipping objects that lie outside the ray’s path.
// ============================================================
class bvh_node : public hittable {
  public:

    // --------------------------------------------------------
    // Constructor: build BVH from a hittable_list
    // Simply delegates to the vector-based constructor.
    // --------------------------------------------------------
    bvh_node(hittable_list list)
        : bvh_node(list.objects, 0, list.objects.size()) {}

    // --------------------------------------------------------
    // BVH builder from a subrange [start, end) of hittable objects.
    // Recursively splits objects along the longest axis of their
    // combined bounding box.
    // --------------------------------------------------------
    bvh_node(std::vector<shared_ptr<hittable>>& objects, size_t start, size_t end) {
        // Compute the bounding box of all objects in this range
        bbox = aabb::empty;
        for (size_t object_index = start; object_index < end; object_index++)
            bbox = aabb(bbox, objects[object_index]->bounding_box());

        // Choose the axis with the largest extent for splitting
        int axis = bbox.longest_axis();

        // Choose comparator based on axis
        auto comparator = (axis == 0) ? box_x_compare
                         : (axis == 1) ? box_y_compare
                         : box_z_compare;

        size_t object_span = end - start;

        // Base cases — small spans become leaf nodes
        if (object_span == 1) {
            // One object → both left and right point to same object
            left = right = objects[start];
        } else if (object_span == 2) {
            // Two objects → directly assign them as children
            left = objects[start];
            right = objects[start + 1];
        } else {
            // Recursive case
            // Sort objects along the chosen axis
            std::sort(std::begin(objects) + start,
                      std::begin(objects) + end,
                      comparator);

            // Split objects into two halves
            auto mid = start + object_span / 2;
            left  = make_shared<bvh_node>(objects, start, mid);
            right = make_shared<bvh_node>(objects, mid, end);
        }
    }

    // --------------------------------------------------------
    // Intersection test with BVH node.
    // First, test the ray against the node’s bounding box.
    // If it misses, skip all child objects.
    // If it hits, recurse into left and right children.
    //
    // Optimization: When checking the right child, shrink the
    // max t-value to the closest hit found so far.
    // --------------------------------------------------------
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        if (!bbox.hit(r, ray_t))
            return false;

        bool hit_left = left->hit(r, ray_t, rec);
        bool hit_right = right->hit(r,
                                    interval(ray_t.min, hit_left ? rec.t : ray_t.max),
                                    rec);

        return hit_left || hit_right;
    }

    // --------------------------------------------------------
    // Returns the bounding box of this node
    // --------------------------------------------------------
    aabb bounding_box() const override { return bbox; }

  private:
    shared_ptr<hittable> left;   // Left child
    shared_ptr<hittable> right;  // Right child
    aabb bbox;                   // Bounding box for this node

    // --------------------------------------------------------
    // Sorting helpers for BVH construction
    // Compare two objects along the given axis by their
    // bounding box minimum coordinate.
    // --------------------------------------------------------
    static bool box_compare(
        const shared_ptr<hittable> a, const shared_ptr<hittable> b, int axis_index
    ) {
        auto a_axis_interval = a->bounding_box().axis_interval(axis_index);
        auto b_axis_interval = b->bounding_box().axis_interval(axis_index);
        return a_axis_interval.min < b_axis_interval.min;
    }

    static bool box_x_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 0);
    }

    static bool box_y_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 1);
    }

    static bool box_z_compare (const shared_ptr<hittable> a, const shared_ptr<hittable> b) {
        return box_compare(a, b, 2);
    }
};

#endif
