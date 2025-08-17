cmake_minimum_required(VERSION 3.10)
project(RayTracer)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# List your source files
set(SOURCES
    src/main.cpp
    src/camera.h
    src/color.h
    src/hittable.h
    src/hittable_list.h
    src/interval.h
    src/ray.h
    src/ray_tracer.h
    src/sphere.h
    src/vector3.h
    src/material.h
    src/input.h
)

# Create the executable
add_executable(RayTracer ${SOURCES})