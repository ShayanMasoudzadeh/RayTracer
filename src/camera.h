#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    double aspect_ratio = 1.0;      // Ratio of image width over height
    int image_width = 100;          // Rendered image width in pixel count
    int samples_per_pixel = 10;     // Count of random samples for each pixel
    int max_depth = 10;             // Maximum number of ray bounces into scene

    void render(const hittable& scene) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i,j);
                    pixel_color += ray_color(r, max_depth, scene);
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }
        std::clog << "\rDone!                       \n";
    }

  private:    
    int image_height;           // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    vector3 center;             // Camera center
    vector3 pixel00_loc;        // Location of pixel 0, 0
    vector3 pixel_delta_u;      // Offset to pixel to the right
    vector3 pixel_delta_v;      // Offset to pixel below


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = vector3(0, 0, 0);
        
        //Determine viewport dimensions.
        double focal_length = 1.0;
        double viewport_height = 2.0;
        double viewport_width = viewport_height * (double(image_width)/image_height);
        //viewport edges
        vector3 viewport_u = vector3(viewport_width, 0, 0);
        vector3 viewport_v = vector3(0, -viewport_height, 0);
        //pixel to pixel distance
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        //viewport top left pixel
        vector3 viewport_upper_left = center - vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the origin and directed at randomly sampled
        // point around the pixel location i, j.

        vector3 offset = sample_square();
        vector3 pixel_sample = pixel00_loc + ((i + offset.x()) * pixel_delta_u) + ((j + offset.y()) * pixel_delta_v);

        vector3 ray_origin = center;
        vector3 ray_direction = pixel_sample - ray_origin;

        return ray(ray_origin, ray_direction);
    }

    vector3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        //return vector3(random_double() - 0.5, random_double() - 0.5, 0);
        return vector3(random_double(-0.5, 0.5), random_double(-0.5, 0.5), 0);
    }

    color ray_color(const ray& r, int depth, const hittable& scene) const {
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;
        if (scene.hit(r, interval(0.001, infinity), rec)) {
            // vector3 direction = random_on_hemisphere(rec.normal);    //uniform diffuse reflection
            vector3 direction = rec.normal + random_unit_vector();      //Lambertian diffuse reflection
            return 0.5 * ray_color(ray(rec.p, direction), depth-1, scene);
        }

        //background color
        vector3 unit_direction = r.direction().normalize();
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif