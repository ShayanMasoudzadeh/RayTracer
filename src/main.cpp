#include "ray_tracer.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "input.h"

int main() {
    hittable_list scene;

    // auto material_ground = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_center = make_shared<lambertian>(color(0.5, 0.1, 0.1));
    // auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
    // auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // scene.add(make_shared<sphere>(vector3( 0.0, -100.5, -1.0), 100.0, material_ground));
    // scene.add(make_shared<sphere>(vector3( 0.0,    0.0, -1.2),   0.5, material_center));
    // scene.add(make_shared<sphere>(vector3(-1.0,    0.0, -1.0),   0.5, material_left));
    // scene.add(make_shared<sphere>(vector3( 1.0,    0.0, -1.0),   0.5, material_right));

    // scene = load_scene_from_file("scene.txt");

    // camera cam;

    // cam.aspect_ratio = 16.0 / 9.0;
    // cam.image_width = 400;
    // cam.samples_per_pixel = 100;
    // cam.max_depth = 10;
    // cam.vfov = 40;
    // cam.lookfrom = vector3(-2,2,1);
    // cam.lookat   = vector3(0,0,-1);
    // cam.vup      = vector3(0,1,0);

    // cam.render(scene);

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    scene.add(make_shared<sphere>(vector3(0,-1000,0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vector3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vector3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    scene.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else  {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    scene.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    scene.add(make_shared<sphere>(vector3(-4, 1, 0), 1.0, material1));

    auto material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<sphere>(vector3(4, 1, 0), 1.0, material2));

    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 20;

    cam.vfov     = 20;
    cam.lookfrom = vector3(13,2,3);
    cam.lookat   = vector3(0,0,0);
    cam.vup      = vector3(0,1,0);

    cam.render(scene);
}