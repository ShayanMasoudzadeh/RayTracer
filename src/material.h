#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracer.h"
#include "hittable.h"

// ------------------------------------------------------
// Base Class: material
// Represents the surface properties of an object.
// Materials define how rays interact with surfaces:
//  - whether they scatter
//  - how much light they emit
//  - the color attenuation of scattered rays
// ------------------------------------------------------
class material {
public:
    virtual ~material() = default;

    // ------------------------------------------------------
    // emitted()
    // Returns the light emitted by the material.
    // Default is black (no emission).
    // Light sources override this method.
    // ------------------------------------------------------
    virtual color emitted() const {
        return color(0,0,0);
    }

    // ------------------------------------------------------
    // scatter()
    // Calculates how an incoming ray interacts with the surface.
    // Outputs:
    //  - attenuation: the fraction of light kept
    //  - scattered: the new scattered ray
    // Returns:
    //  - true if ray scatters, false if absorbed
    // Default implementation: absorbs everything (false).
    // ------------------------------------------------------
    virtual bool scatter(
        const ray& r_in,
        const hit_record& rec,
        color& attenuation,
        ray& scattered
    ) const {
        return false;
    }

    // ------------------------------------------------------
    // log()
    // Outputs material info for debugging/logging.
    // Default does nothing.
    // ------------------------------------------------------
    virtual void log() const {}
};

// ------------------------------------------------------
// Class: lambertian
// Diffuse material that scatters light evenly in all directions
// (Lambertian reflection).
// ------------------------------------------------------
class lambertian : public material {
public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(
        const ray& r_in,
        const hit_record& rec,
        color& attenuation,
        ray& scattered
    ) const override {
        // Random scatter direction for diffuse reflection
        vector3 scatter_direction = rec.normal + random_unit_vector();

        // Handle the case where scatter_direction is nearly zero
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;

        // Create scattered ray
        scattered = ray(rec.p, scatter_direction);

        // Surface color attenuation
        attenuation = albedo;

        return true;
    }

    // Debug log for color
    void log() const override {
        std::clog << "\n(" << albedo.x() << ", " << albedo.y() << ", " << albedo.z() << ")";
    }

private:
    color albedo; // Base surface color
};

// ------------------------------------------------------
// Class: metal
// Reflective material that produces mirror-like reflection.
// Includes a fuzz factor to simulate imperfect surfaces.
// ------------------------------------------------------
class metal : public material {
public:
    metal(const color& albedo, double fuzz)
        : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(
        const ray& r_in,
        const hit_record& rec,
        color& attenuation,
        ray& scattered
    ) const override {
        // Reflect the incoming ray around the surface normal
        vector3 reflected = reflect(r_in.direction(), rec.normal);
        
        // Normalize and add fuzz for imperfect reflection
        reflected = reflected.normalize() + (fuzz * random_unit_vector());

        // Create scattered ray
        scattered = ray(rec.p, reflected);

        // Color attenuation
        attenuation = albedo;

        // Scatter only if reflection is above the surface
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    // Debug log for color
    void log() const override {
        std::clog << "\n\n(" << albedo.x() << ", " << albedo.y() << ", " << albedo.z() << ")\n\n";
    }

private:
    color albedo; // Surface color
    double fuzz;  // Reflection fuzziness [0 = perfect mirror]
};

// ------------------------------------------------------
// Class: diffuse_light
// Light-emitting material with a constant color.
// Rays do not scatter; they just emit light.
// ------------------------------------------------------
class diffuse_light : public material {
public:
    diffuse_light(const color& albedo) : albedo(albedo) {}

    // Emit light with the given color
    color emitted() const override {
        return albedo;
    }

private:
    color albedo; // Emitted light color
};

#endif // MATERIAL_H
