#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "aabb.h"
#include "hittable.h"

#include <vector>

// ------------------------------------------------------
// Class: hittable_list
// A container for multiple hittable objects.
// Implements the hittable interface, so the entire list
// can be treated as a single hittable object.
// 
// Responsibilities:
//  - Store and manage a collection of objects
//  - Forward ray intersection checks to each object
//  - Keep track of an overall bounding box for acceleration
// ------------------------------------------------------
class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects; // All objects in the list

    // Default constructor: creates an empty list
    hittable_list() {}

    // Constructor with a single object
    hittable_list(shared_ptr<hittable> object) { add(object); }
    
    // Remove all objects
    void clear() { objects.clear(); }

    // Add a new hittable object to the list
    // Also updates the overall bounding box
    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box()); // Expand bbox to include new object
    }

    // ------------------------------------------------------
    // Ray intersection test for the entire list
    // Iterates over all objects and finds the closest hit.
    // 
    // Parameters:
    //  - r: the ray being tested
    //  - ray_t: interval for valid t-values
    //  - rec: hit record to store intersection info
    //
    // Returns: true if any object is hit, false otherwise
    // ------------------------------------------------------
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max; // Tracks the closest hit so far

        // Check each object
        for (const auto& object : objects) {
            // Narrow ray_t to avoid hits farther than closest_so_far
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t; // Update closest hit distance
                rec = temp_rec;              // Save hit details
            }
        }

        return hit_anything;
    }

    // Return the combined bounding box of all objects
    aabb bounding_box() const { return bbox; }

private:
    aabb bbox; // Overall bounding box for the list
};

#endif // HITTABLE_LIST_H
