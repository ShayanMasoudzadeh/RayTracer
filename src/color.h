#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <algorithm>

class color {
public:
    unsigned __int8 r; //red channel
    unsigned __int8 g; //green channel
    unsigned __int8 b; //blue channel

    //constructors
    color::color(int red, int green, int blue) : r(red), g(green), b(blue) {}
    color::color(){
	    r = 0;
	    g = 0;
	    b = 0;
    }
};

inline color operator+(const color& c1, const color& c2){
    return color(
        std::min(255, std::max(0, c1.r + c2.r)),
        std::min(255, std::max(0, c1.g + c2.g)),
        std::min(255, std::max(0, c1.b + c2.b))
    );
}

inline color operator*(const color& c, double scalar){
    return color(
        std::min(255, std::max(0, static_cast<int>(c.r * scalar))),
        std::min(255, std::max(0, static_cast<int>(c.g * scalar))),
        std::min(255, std::max(0, static_cast<int>(c.b * scalar)))
    );
}

void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.r;
    auto g = pixel_color.g;
    auto b = pixel_color.b;

    // Translate the [0,1] component values to the byte range [0,255].
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif