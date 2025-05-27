#ifndef CAMERA_H
#define CAMERA_H

#include <thread>

#include "hittable.h"
#include "material.h"

class camera {
  public:
    double aspect_ratio = 1.0;      // Ratio of image width over height
    int image_width = 100;          // Rendered image width in pixel count
    int samples_per_pixel = 10;     // Count of random samples for each pixel
    int max_depth = 10;             // Maximum number of ray bounces into scene
    color  background;              // Scene background color

    double vfov = 90;                   // Vertical view angle (field of view)
    vector3 lookfrom = vector3(0,0,0);  // Point camera is looking from
    vector3 lookat = vector3(0,0,-1);   // Point camera is looking at
    vector3 vup = vector3(0,1,0);       // Camera-relative "up" direction

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

    void render_parallel(const hittable& scene){
        initialize();

        std::vector<std::thread> threads;
        int thread_count = std::thread::hardware_concurrency();
        int rows_per_thread = image_height / thread_count;

        std::vector<std::vector<color>> framebuffer(image_height, std::vector<color>(image_width));

        for (int t = 0; t < thread_count; t++) {
            int row_start = t * rows_per_thread;
            int row_end = (t == thread_count - 1) ? image_height : row_start + rows_per_thread;

            threads.emplace_back(&camera::render_rows, this, row_start, row_end, std::ref(scene), std::ref(framebuffer));
        }

        for (auto& thread : threads) thread.join();

        // Output image (in main thread only!)
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                write_color(std::cout, framebuffer[j][i] * pixel_samples_scale);
            }
        }
    }
    void render_rows(int start_row, int end_row, const hittable& scene, std::vector<std::vector<color>>& framebuffer){
        for (int j = start_row; j < end_row; j++) {
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0, 0, 0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, scene);
                }
                framebuffer[j][i] = pixel_color;
            }
        }
    }

  private:    
    int image_height;           // Rendered image height
    double pixel_samples_scale; // Color scale factor for a sum of pixel samples
    vector3 center;             // Camera center
    vector3 pixel00_loc;        // Location of pixel 0, 0
    vector3 pixel_delta_u;      // Offset to pixel to the right
    vector3 pixel_delta_v;      // Offset to pixel below
    vector3   u, v, w;              // Camera frame basis vectors


    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;
        
        //Determine viewport dimensions.
        double focal_length = 1.0;
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focal_length;
        double viewport_width = viewport_height * (double(image_width)/image_height);
        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = (lookfrom - lookat).normalize();
        u = (cross(vup, w)).normalize();
        v = cross(w, u);
        //viewport edges
        vector3 viewport_u = viewport_width * u;
        vector3 viewport_v = viewport_height * -v;
        //pixel to pixel distance
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;
        //viewport top left pixel
        vector3 viewport_upper_left = center - (focal_length * w) - viewport_u/2 - viewport_v/2;
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

        // If the ray hits nothing, return the background color.
        if (!scene.hit(r, interval(0.001, infinity), rec)) 
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted();
        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;
        color color_from_scatter = attenuation * ray_color(scattered, depth-1, scene);
        
        return color_from_emission + color_from_scatter;
    }
};

#endif