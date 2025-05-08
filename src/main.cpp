#include "ray_tracer.h"

#include "camera.h"
#include "hittable.h"
#include "sphere.h"
#include "hittable_list.h"

int main() {
    hittable_list scene;
    scene.add(make_shared<sphere>(vector3(0,0,-1), 0.5));
    scene.add(make_shared<sphere>(vector3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 50;

    cam.render(scene);
}