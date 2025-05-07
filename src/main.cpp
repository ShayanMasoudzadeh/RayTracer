#include "ray_tracer.h"

#include "hittable.h"
#include "sphere.h"
#include "hittable_list.h"


color ray_color(const ray& r, const hittable& scene) {
    hit_record rec;
    if (scene.hit(r, interval(0, infinity), rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    //background color
    vector3 unit_direction = r.direction().normalize();
    double a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

int main() {

    // Image:
    double aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Scene:
    hittable_list scene;
    scene.add(make_shared<sphere>(vector3(0,0,-1), 0.5));
    scene.add(make_shared<sphere>(vector3(0,-100.5,-1), 100));

    // Camera:
    double focal_length = 1.0;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(image_width)/image_height);
    vector3 camera_center = vector3(0, 0, 0);

    //viewport edges
    vector3 viewport_u = vector3(viewport_width, 0, 0);
    vector3 viewport_v = vector3(0, -viewport_height, 0);
    //pixel to pixel distance
    vector3 pixel_delta_u = viewport_u / image_width;
    vector3 pixel_delta_v = viewport_v / image_height;
    //viewport top left pixel
    vector3 viewport_upper_left = camera_center - vector3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    vector3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render:
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush; 
        for (int i = 0; i < image_width; i++) {
            vector3 pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            vector3 ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r, scene);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone!                       \n";
}