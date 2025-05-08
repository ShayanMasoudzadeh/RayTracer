#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
  public:
    double aspect_ratio = 1.0;  // Ratio of image width over height
    int image_width = 100;      // Rendered image width in pixel count

    void render(const hittable& scene) {
        initialize();

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
            for (int i = 0; i < image_width; i++) {
                vector3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
                vector3 ray_direction = pixel_center - center;
                ray r(center, ray_direction);

                color pixel_color = ray_color(r, scene);
                write_color(std::cout, pixel_color);
            }
        }
        std::clog << "\rDone!                       \n";
    }

  private:    
    int image_height;       // Rendered image height
    vector3 center;         // Camera center
    vector3 pixel00_loc;    // Location of pixel 0, 0
    vector3 pixel_delta_u;  // Offset to pixel to the right
    vector3 pixel_delta_v;  // Offset to pixel below


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
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

    color ray_color(const ray& r, const hittable& scene) const {
        hit_record rec;
        if (scene.hit(r, interval(0, infinity), rec)) {
            return 0.5 * (rec.normal + color(1,1,1));
        }

        //background color
        vector3 unit_direction = r.direction().normalize();
        double a = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
    }
};

#endif