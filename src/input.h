#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "ray_tracer.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "tri.h"

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

void load_obj_file(const std::string& filename, hittable_list& scene, shared_ptr<material> mat) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open OBJ file: " << filename << "\n";
        return;
    }

    std::vector<vector3> vertices;
    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        iss >> token;

        if (token == "v") {
            double x, y, z;
            iss >> x >> y >> z;
            vertices.emplace_back(x, y, z);
        }
        else if (token == "f") {
            int i1, i2, i3;
            iss >> i1 >> i2 >> i3;
            // OBJ is 1-based, so subtract 1
            scene.add(make_shared<tri>(
                vertices[i1 - 1],
                vertices[i2 - 1],
                vertices[i3 - 1],
                mat
            ));
        }
    }
}