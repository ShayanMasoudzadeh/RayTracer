#ifndef CAMERA_H
#define CAMERA_H

#include <thread>

#include "hittable.h"
#include "material.h"

// ------------------------------------------------------
// Class: camera
// A ray tracing camera with configurable parameters,
// supporting sequential and multi-threaded rendering.
// ------------------------------------------------------
class camera {
public:
    // --- Public settings (configurable before render) ---
    double aspect_ratio = 1.0;      // Ratio of image width to height
    int image_width = 100;          // Width in pixels
    int samples_per_pixel = 10;     // Anti-aliasing samples per pixel
    int max_depth = 10;             // Max recursion depth for ray bounces
    color background;               // Background color when ray hits nothing

    // Camera positioning/orientation parameters
    double vfov = 90;                   // Vertical field of view in degrees
    vector3 lookfrom = vector3(0,0,0);  // Camera position
    vector3 lookat   = vector3(0,0,-1); // Target point the camera looks at
    vector3 vup      = vector3(0,1,0);  // "Up" direction for camera orientation

    // ------------------------------------------------------
    // render(scene)
    // Sequential render. Loops through all pixels, computes
    // multiple samples for anti-aliasing, and outputs PPM.
    // ------------------------------------------------------
    void render(const hittable& scene) {
        initialize(); // Compute camera parameters

        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

        for (int j = 0; j < image_height; j++) {
            std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
            for (int i = 0; i < image_width; i++) {
                color pixel_color(0,0,0);
                for (int sample = 0; sample < samples_per_pixel; sample++) {
                    ray r = get_ray(i, j);
                    pixel_color += ray_color(r, max_depth, scene);
                }
                write_color(std::cout, pixel_color * pixel_samples_scale);
            }
        }
        std::clog << "\rDone!                       \n";
    }

    // ------------------------------------------------------
    // render_parallel(scene)
    // Multi-threaded version of render(). Splits the image
    // into horizontal bands and assigns each to a thread.
    // ------------------------------------------------------
    void render_parallel(const hittable& scene) {
        initialize();

        std::vector<std::thread> threads;
        int thread_count = std::thread::hardware_concurrency();
        int rows_per_thread = image_height / thread_count;

        // Framebuffer: 2D array to store colors from each thread
        std::vector<std::vector<color>> framebuffer(
            image_height, std::vector<color>(image_width)
        );

        // Launch threads, each processing its own row range
        for (int t = 0; t < thread_count; t++) {
            int row_start = t * rows_per_thread;
            int row_end = (t == thread_count - 1) ? image_height : row_start + rows_per_thread;

            threads.emplace_back(
                &camera::render_rows, this, row_start, row_end,
                std::ref(scene), std::ref(framebuffer)
            );
        }

        // Wait for all threads to finish
        for (auto& thread : threads) thread.join();

        // Output the image (done only once from main thread)
        std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
        for (int j = 0; j < image_height; j++) {
            for (int i = 0; i < image_width; i++) {
                write_color(std::cout, framebuffer[j][i] * pixel_samples_scale);
            }
        }
    }

    // ------------------------------------------------------
    // render_rows(start_row, end_row, scene, framebuffer)
    // Helper for render_parallel(). Computes pixel colors
    // for the given row range and stores results in framebuffer.
    // ------------------------------------------------------
    void render_rows(int start_row, int end_row,
                     const hittable& scene,
                     std::vector<std::vector<color>>& framebuffer) {
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
    // --- Derived internal variables ---
    int image_height;           // Computed from aspect ratio
    double pixel_samples_scale; // Normalization factor for sample averaging
    vector3 center;             // Camera position in world space
    vector3 pixel00_loc;        // Location of the top-left pixel center
    vector3 pixel_delta_u;      // Offset from one pixel to the next in x
    vector3 pixel_delta_v;      // Offset from one pixel to the next in y
    vector3 u, v, w;            // Camera coordinate system basis vectors

    // ------------------------------------------------------
    // initialize()
    // Precomputes camera geometry based on parameters.
    // Called before any rendering loop starts.
    // ------------------------------------------------------
    void initialize() {
        // Compute image height from width & aspect ratio
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;
        
        pixel_samples_scale = 1.0 / samples_per_pixel;
        center = lookfrom;

        // Determine viewport dimensions
        double focal_length = 1.0; // Distance from camera to image plane
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta / 2);
        auto viewport_height = 2 * h * focal_length;
        double viewport_width = viewport_height * (double(image_width) / image_height);

        // Camera coordinate frame (u = right, v = up, w = back)
        w = (lookfrom - lookat).normalize();
        u = (cross(vup, w)).normalize();
        v = cross(w, u);

        // Viewport edge vectors in world space
        vector3 viewport_u = viewport_width * u;  // Horizontal axis
        vector3 viewport_v = viewport_height * -v; // Vertical axis (negative = downward)

        // Pixel-to-pixel step vectors
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Location of center of top-left pixel
        vector3 viewport_upper_left =
            center - (focal_length * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
    }

    // ------------------------------------------------------
    // get_ray(i, j)
    // Returns a ray from the camera through pixel (i,j)
    // with random subpixel sampling for anti-aliasing.
    // ------------------------------------------------------
    ray get_ray(int i, int j) const {
        vector3 offset = sample_square(); // Random jitter inside pixel
        vector3 pixel_sample =
            pixel00_loc
            + ((i + offset.x()) * pixel_delta_u)
            + ((j + offset.y()) * pixel_delta_v);

        vector3 ray_origin = center;
        vector3 ray_direction = pixel_sample - ray_origin;
        return ray(ray_origin, ray_direction);
    }

    // ------------------------------------------------------
    // sample_square()
    // Returns a random offset in the unit square [-0.5,0.5]
    // for stochastic sampling within a pixel.
    // ------------------------------------------------------
    vector3 sample_square() const {
        return vector3(random_double(-0.5, 0.5),
                       random_double(-0.5, 0.5),
                       0);
    }

    // ------------------------------------------------------
    // ray_color(ray, depth, scene)
    // Recursively computes the color returned by a ray.
    // Stops when:
    //  - max_depth is reached
    //  - ray hits nothing (returns background color)
    //  - material does not scatter
    // ------------------------------------------------------
    color ray_color(const ray& r, int depth, const hittable& scene) const {
        if (depth <= 0)
            return color(0,0,0); // No more light contribution

        hit_record rec;

        // Ray misses: return background
        if (!scene.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted();

        // If material absorbs light, only emission contributes
        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        // Combine emission + reflected/refracted light
        color color_from_scatter = attenuation * ray_color(scattered, depth - 1, scene);
        return color_from_emission + color_from_scatter;
    }
};

#endif // CAMERA_H