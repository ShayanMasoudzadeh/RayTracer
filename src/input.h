#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "ray_tracer.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "tri.h"

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
        } else if (type == "obj") {
            std::string obj_path;
            std::string mat_type;
            double r, g, b;

            iss >> obj_path >> mat_type >> r >> g >> b;

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

            load_obj_file(obj_path, scene, mat);
        } else {
            std::cerr << "Unknown object type: " << type << "\n";
        }
    }

    return scene;
}

void set_camera(const std::string& filename, camera& cam) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Failed to open camera settings file: " << filename << "\n";
        return;
    }

    std::string key;
    while (file >> key) {
        if (key == "aspect_ratio") {
            file >> cam.aspect_ratio;
        } else if (key == "image_width") {
            file >> cam.image_width;
        } else if (key == "samples_per_pixel") {
            file >> cam.samples_per_pixel;
        } else if (key == "max_depth") {
            file >> cam.max_depth;
        } else if (key == "vfov") {
            file >> cam.vfov;
        } else if (key == "lookfrom") {
            double x, y, z;
            file >> x >> y >> z;
            cam.lookfrom = vector3(x, y, z);
        } else if (key == "lookat") {
            double x, y, z;
            file >> x >> y >> z;
            cam.lookat = vector3(x, y, z);
        } else if (key == "vup") {
            double x, y, z;
            file >> x >> y >> z;
            cam.vup = vector3(x, y, z);
        } else {
            std::cerr << "Unknown camera setting: " << key << "\n";
            std::string dummy;
            std::getline(file, dummy); // skip line
        }
    }
}