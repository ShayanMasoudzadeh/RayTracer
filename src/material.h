#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray_tracer.h"
#include "hittable.h"

class material {
    public:

    virtual ~material() = default;

    virtual color emitted() const {
        return color(0,0,0);
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
        return false;
    }

    virtual void log() const {}
};

class lambertian : public material {
    public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter (const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        //vector3 scatter_direction = random_on_hemisphere(rec.normal); //uniform diffuse reflection
        vector3 scatter_direction = rec.normal + random_unit_vector();  //Lambertian diffuse reflection
        if (scatter_direction.near_zero())
            scatter_direction = rec.normal;
        scattered = ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    void log() const override {
        std::clog << "\n(" << albedo.x() << ", " << albedo.y() << ", " << albedo.z() << ")";
    }

    private:
    color albedo;
};

class metal : public material {
    public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1? fuzz : 1) {}

    bool scatter (const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
        vector3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = reflected.normalize() + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

    void log() const override {
        std::clog << "\n\n(" << albedo.x() << ", " << albedo.y() << ", " << albedo.z() << ")\n\n";
    }

    private:
    color albedo;
    double fuzz;
};

class diffuse_light : public material {
  public:
    diffuse_light(const color& albedo) : albedo(albedo) {}

    color emitted() const override {
        return albedo;
    }

  private:
    color albedo;
};

#endif