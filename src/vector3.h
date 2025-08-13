#ifndef VECTOR3_H
#define VECTOR3_H

// ------------------------------------------------------
// Class: vector3
// A simple 3D vector class with common operations
// used for geometry, colors, and directions.
// ------------------------------------------------------
class vector3 {
public:
    double e[3]; // x, y, z components

    // Constructors
    vector3() : e{0,0,0} {}
    vector3(double x, double y, double z) : e{x, y, z} {}
    
    // Accessors
    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    // Unary minus
    vector3 operator -() const { return vector3(-e[0], -e[1], -e[2]); }

    // Index access
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }
    
    // Compound addition
    vector3& operator+=(const vector3& v) {
        e[0] += v.e[0]; e[1] += v.e[1]; e[2] += v.e[2];
        return *this;
    }

    // Scalar multiplication
    vector3& operator*=(double d) {
        e[0] *= d; e[1] *= d; e[2] *= d;
        return *this;
    }

    // Length and squared length
    double length() const { return std::sqrt(length_squared()); }
    double length_squared() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }

    // Checks if vector is very close to zero in all components
    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    // Random vector in [0,1) or [min,max)
    static vector3 random() { return vector3(random_double(), random_double(), random_double()); }
    static vector3 random(double min, double max) {
        return vector3(random_double(min,max), random_double(min,max), random_double(min,max));
    }
    
    // Return a normalized version of the vector
    vector3 normalize() const {
        double len = length();
        return vector3(e[0] / len, e[1] / len, e[2] / len);
    }
};

// ------------------------------------------------------
// Free functions for vector arithmetic
// ------------------------------------------------------
inline vector3 operator+(const vector3& v, const vector3& u) {
    return vector3(v.e[0] + u.e[0], v.e[1] + u.e[1], v.e[2] + u.e[2]);
}

inline vector3 operator-(const vector3& v, const vector3& u) {
    return vector3(v.e[0] - u.e[0], v.e[1] - u.e[1], v.e[2] - u.e[2]);
}

inline vector3 operator*(const vector3& v, const vector3& u) {
    return vector3(v.e[0] * u.e[0], v.e[1] * u.e[1], v.e[2] * u.e[2]);
}

inline vector3 operator*(double d, const vector3& v) {
    return vector3(v.e[0] * d, v.e[1] * d, v.e[2] * d);
}
inline vector3 operator*(const vector3& v, double d) { return d * v; }

inline vector3 operator/(const vector3& v, double d) { return (1/d) * v; }

// ------------------------------------------------------
// Vector math helpers
// ------------------------------------------------------
inline double dot(const vector3& v, const vector3& u) {
    return v.e[0]*u.e[0] + v.e[1]*u.e[1] + v.e[2]*u.e[2];
}

inline vector3 cross(const vector3& v, const vector3& u) {
    return vector3(
        v.e[1]*u.e[2] - v.e[2]*u.e[1],
        v.e[2]*u.e[0] - v.e[0]*u.e[2],
        v.e[0]*u.e[1] - v.e[1]*u.e[0]
    );
}

// Generate a random unit vector uniformly on a sphere
inline vector3 random_unit_vector() {
    while (true) {
        auto p = vector3::random(-1, 1);
        auto lensq = p.length_squared();
        if (1e-160 < lensq && lensq <= 1)
            return p / sqrt(lensq);
    }
}

// Generate a random unit vector in the same hemisphere as 'normal'
inline vector3 random_on_hemisphere(const vector3& normal) {
    vector3 on_unit_sphere = random_unit_vector();
    return (dot(on_unit_sphere, normal) > 0.0) ? on_unit_sphere : -on_unit_sphere;
}

// Reflect vector v around normal n
inline vector3 reflect(const vector3& v, const vector3& n) {
    return v - 2 * dot(v, n) * n;
}

#endif // VECTOR3_H