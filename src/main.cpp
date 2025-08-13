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

// --------------------------------------
// Scene: Many Spheres
// Creates a ground plane sphere and a grid of random spheres
// with either diffuse or metallic materials, then renders the scene.
// --------------------------------------
void many_spheres() {
    hittable_list scene;

    // Ground material (large sphere as the floor)
    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    scene.add(make_shared<sphere>(vector3(0,-1000,0), 1000, ground_material));

    // Generate random small spheres
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vector3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            // Avoid placing spheres too close to the big sphere at (4, 0.2, 0)
            if ((center - vector3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // Diffuse sphere
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                } else {
                    // Metallic sphere
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                }

                scene.add(make_shared<sphere>(center, 0.2, sphere_material));
            }
        }
    }

    // Three large example spheres
    auto material1 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    scene.add(make_shared<sphere>(vector3(-4, 1, 0), 1.0, material1));

    auto material2 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    scene.add(make_shared<sphere>(vector3(4, 1, 0), 1.0, material2));

    // Use a BVH (Bounding Volume Hierarchy) for faster rendering
    scene = hittable_list(make_shared<bvh_node>(scene));

    // Camera setup
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 20;
    cam.vfov              = 20;
    cam.lookfrom          = vector3(13,2,3);
    cam.lookat            = vector3(0,0,0);
    cam.vup               = vector3(0,1,0);
    cam.background        = color(0.7,0.8,1);

    // Render the scene
    cam.render(scene);
}

// --------------------------------------
// Scene: Three Spheres
// A simple scene with a ground sphere and three different materials.
// --------------------------------------
void three_spheres() {
    hittable_list scene;

    // Materials
    auto material_ground = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_center = make_shared<lambertian>(color(0.5, 0.1, 0.1));
    auto material_left   = make_shared<metal>(color(0.8, 0.8, 0.8), 0.1);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 1.0);

    // Objects
    scene.add(make_shared<sphere>(vector3( 0.0, -100.5, -1.0), 100.0, material_ground));
    scene.add(make_shared<sphere>(vector3( 0.0,    0.0, -1.2),   0.5, material_center));
    scene.add(make_shared<sphere>(vector3(-1.0,    0.0, -1.0),   0.5, material_left));
    scene.add(make_shared<sphere>(vector3( 1.0,    0.0, -1.0),   0.5, material_right));

    // Camera setup
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 10;
    cam.vfov              = 90;
    cam.lookfrom          = vector3(0, 0, 0);
    cam.lookat            = vector3(0, 0, -1);
    cam.vup               = vector3(0, 1, 0);
    cam.background        = color(0.7,0.8,1);

    cam.render(scene);
}

// --------------------------------------
// Scene: Triangles
// Creates colored triangles forming a cube-like enclosure.
// --------------------------------------
void tris() {
    hittable_list scene;

    // Materials for each wall
    auto left_red     = make_shared<lambertian>(color(1.0, 0.2, 0.2));
    auto back_green   = make_shared<lambertian>(color(0.2, 1.0, 0.2));
    auto right_blue   = make_shared<lambertian>(color(0.2, 0.2, 1.0));
    auto upper_orange = make_shared<lambertian>(color(1.0, 0.5, 0.0));
    auto lower_teal   = make_shared<lambertian>(color(0.2, 0.8, 0.8));

    // Add triangle faces (two per wall)
    scene.add(make_shared<tri>(vector3(-3,-2, 5), vector3(-3, -2, 1), vector3(-3, 2, 5), left_red));
    scene.add(make_shared<tri>(vector3(-3,2, 1),  vector3(-3, -2, 1), vector3(-3, 2, 5), left_red));

    scene.add(make_shared<tri>(vector3(-2,-2, 0), vector3(2, -2, 0),  vector3(-2, 2, 0), back_green));
    scene.add(make_shared<tri>(vector3(2,2, 0),   vector3(2, -2, 0),  vector3(-2, 2, 0), back_green));

    scene.add(make_shared<tri>(vector3(3,-2, 1),  vector3(3,-2, 5),   vector3(3, 2, 1), right_blue));
    scene.add(make_shared<tri>(vector3(3,2, 5),   vector3(3,-2, 5),   vector3(3, 2, 1), right_blue));

    scene.add(make_shared<tri>(vector3(-2, 3, 1), vector3(2, 3, 1),   vector3(-2, 3, 5), upper_orange));
    scene.add(make_shared<tri>(vector3(2, 3, 5),  vector3(2, 3, 1),   vector3(-2, 3, 5), upper_orange));

    scene.add(make_shared<tri>(vector3(-2,-3, 5), vector3(2,-3, 5),   vector3(-2,-3, 1), lower_teal));
    scene.add(make_shared<tri>(vector3(2,-3, 1),  vector3(2,-3, 5),   vector3(-2,-3, 1), lower_teal));

    // Camera setup
    camera cam;
    cam.aspect_ratio      = 1.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.vfov              = 80;
    cam.lookfrom          = vector3(0,0,9);
    cam.lookat            = vector3(0,0,0);
    cam.vup               = vector3(0,1,0);
    cam.background        = color(0.7,0.8,1);

    cam.render(scene);
}

// --------------------------------------
// Scene: Custom Scene from File
// Loads objects and camera settings from external txt files.
// --------------------------------------
void custom_scene() {
    hittable_list scene = load_scene_from_file("custom_scene.txt");
    scene = hittable_list(make_shared<bvh_node>(scene));

    camera cam;
    set_camera("camera_settings.txt", cam);

    // Parallel rendering for faster output
    cam.render_parallel(scene);
}

// --------------------------------------
// Main entry point
// Logs start and end, then selects a scene to render.
// --------------------------------------
int main() {
    Logger logger;

    logger.log("Rendering started.");

    switch (4) {  // Selects which scene to render (hardcoded to 6)
        case 1:  many_spheres();   break;
        case 2:  three_spheres();  break;
        case 3:  tris();           break;
        case 4:  custom_scene();   break;
    }

    logger.log("Rendering finished.");
}
