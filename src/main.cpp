#include "ray_tracer.h"

#include "bvh.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "tri.h"
#include "input.h"
#include "log.h"

void many_spheres() {
    hittable_list scene;

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

void three_spheres() {
    hittable_list scene;

    auto material_ground = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center = make_shared<lambertian>(color(0.5, 0.1, 0.1));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    scene.add(make_shared<sphere>(vector3( 0.0, -100.5, -1.0), 100.0, material_ground));
    scene.add(make_shared<sphere>(vector3( 0.0,    0.0, -1.2),   0.5, material_center));
    scene.add(make_shared<sphere>(vector3(-1.0,    0.0, -1.0),   0.5, material_left));
    scene.add(make_shared<sphere>(vector3( 1.0,    0.0, -1.0),   0.5, material_right));

    //scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;
    cam.vfov = 90;
    cam.lookfrom = vector3(0, 0, 0);
    cam.lookat   = vector3(0, 0, -1);
    cam.vup      = vector3(0, 1, 0);

    cam.render(scene);
}

void load_file() {
    hittable_list scene;

    scene = load_scene_from_file("scene.txt");

    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;
    cam.vfov = 40;
    cam.lookfrom = vector3(-2,2,1);
    cam.lookat   = vector3(0,0,-1);
    cam.vup      = vector3(0,1,0);

    cam.render(scene);
}

void tris() {
    hittable_list scene;

    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    scene.add(make_shared<tri>(vector3(-3,-2, 5), vector3(-3, -2, 1), vector3(-3, 2, 5), left_red));
    scene.add(make_shared<tri>(vector3(-3,2, 1), vector3(-3, -2, 1), vector3(-3, 2, 5), left_red));
    scene.add(make_shared<tri>(vector3(-2,-2, 0), vector3(2, -2, 0), vector3(-2, 2, 0), back_green));
    scene.add(make_shared<tri>(vector3(2,2, 0), vector3(2, -2, 0), vector3(-2, 2, 0), back_green));
    scene.add(make_shared<tri>(vector3(3,-2, 1), vector3(3,-2, 5), vector3(3, 2, 1), right_blue));
    scene.add(make_shared<tri>(vector3(3,2, 5), vector3(3,-2, 5), vector3(3, 2, 1), right_blue));
    scene.add(make_shared<tri>(vector3(-2, 3, 1), vector3(2, 3, 1), vector3(-2, 3, 5), upper_orange));
    scene.add(make_shared<tri>(vector3(2, 3, 5), vector3(2, 3, 1), vector3(-2, 3, 5), upper_orange));
    scene.add(make_shared<tri>(vector3(-2,-3, 5), vector3(2,-3, 5), vector3(-2,-3, 1), lower_teal));
    scene.add(make_shared<tri>(vector3(2,-3, 1), vector3(2,-3, 5), vector3(-2,-3, 1), lower_teal));

    camera cam;

    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 80;
    cam.lookfrom = vector3(0,0,9);
    cam.lookat   = vector3(0,0,0);
    cam.vup      = vector3(0,1,0);

    cam.render(scene);
}

void load_obj() {
    hittable_list scene;

    auto material = make_shared<lambertian>(color(0.5, 0.1, 0.1));

    load_obj_file("obj/model.obj", scene, material);

    auto material_right  = make_shared<metal>(color(0.8, 0.8, 0.8), 0.05);
    scene.add(make_shared<sphere>(vector3( 2,    0.0, 0),   0.6, material_right));

    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 10;
    cam.vfov = 40;
    cam.lookfrom = vector3(0,2,5);
    cam.lookat   = vector3(0,0,0);
    cam.vup      = vector3(0,1,0);

    cam.render(scene);
}

void custom_scene() {
    hittable_list scene;

    scene = load_scene_from_file("custom_scene.txt");
    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;
    set_camera("camera_settings.txt", cam);

    cam.render_parallel(scene);
}

int main() {
    Logger logger;

    logger.log("Rendering started.");
    switch (6) {
        case 1:  many_spheres();   break;
        case 2:  three_spheres();  break;
        case 3:  load_file();              break;
        case 4:  tris();     break;
        case 5:  load_obj();     break;
        case 6: custom_scene(); break;
    }
    logger.log("Rendering finished.");
}