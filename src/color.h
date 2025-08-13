#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vector3.h"

// ------------------------------------------------------
// Color Type Alias
// A color is simply a 3D vector (RGB values).
// vector3.x = red, vector3.y = green, vector3.z = blue
// ------------------------------------------------------
using color = vector3;

// ------------------------------------------------------
// linear_to_gamma()
// Converts a linear color value to gamma-corrected value
// for gamma = 2.0. This helps produce images that look
// correct on standard displays.
// Formula: gamma_corrected = sqrt(linear_component)
// ------------------------------------------------------
inline double linear_to_gamma(double linear_component)
{
    if (linear_component > 0)
        return std::sqrt(linear_component); // gamma correction
    return 0; // clamp negative values to 0
}

// ------------------------------------------------------
// write_color()
// Writes a color value to an output stream in PPM format.
// Steps:
//   1. Extract RGB components from the color vector.
//   2. Apply gamma correction.
//   3. Clamp values to the [0, 0.999] range.
//   4. Convert from [0,1] to integer [0,255] range.
//   5. Output as space-separated integers for PPM.
// ------------------------------------------------------
void write_color(std::ostream& out, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Apply gamma correction (gamma = 2.0)
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // Clamp values to avoid overflow at exactly 1.0
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Output in "R G B" format for PPM image
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif // COLOR_H
