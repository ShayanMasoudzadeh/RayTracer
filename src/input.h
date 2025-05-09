#include <fstream>
#include <sstream>
#include <string>

#include "ray_tracer.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

hittable_list load_scene_from_file(const std::string& filename) {
    hittable_list scene;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);

        std::string type;
        iss >> type;

        if (type == "sphere") {
            double x, y, z, radius;
            std::string mat_type;
            double r, g, b;

            iss >> x >> y >> z >> radius >> mat_type >> r >> g >> b;

            shared_ptr<material> mat;

            if (mat_type == "lambertian") {
                mat = make_shared<lambertian>(color(r, g, b));
            } else if (mat_type == "metal") {
                double fuzz;
                iss >> fuzz;
                mat = make_shared<metal>(color(r, g, b), fuzz);
            } else {
                std::cerr << "Unknown material type: " << mat_type << "\n";
                continue;
            }

            scene.add(make_shared<sphere>(vector3(x, y, z), radius, mat));
        } else {
            std::cerr << "Unknown object type: " << type << "\n";
        }
    }

    return scene;
}
