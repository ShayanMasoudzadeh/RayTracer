# cmake_minimum_required(VERSION 3.10)
# project(RayTracer)

# set(CMAKE_CXX_STANDARD 17)
# add_executable(RayTracer src/main.cpp)

cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List your source files
set(SOURCES
    src/main.cpp
    src/ray_tracer.h
    src/vector3.h
    src/ray.h
    src/hittable.h
    src/hittable_list.h
    src/sphere.h
    src/color.h
)

# Create the executable
add_executable(RayTracer ${SOURCES})