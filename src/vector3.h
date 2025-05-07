#ifndef VECTOR3_H
#define VECTOR3_H


class vector3 {
    public:
    double e[3];

    vector3() : e{0,0,0} {}
    vector3(double x, double y, double z) : e{x, y, z} {}
    
    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vector3 operator -() const {return vector3(-e[0], -e[1], -e[2]);}
    vector3& operator+= (const vector3& v){
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }
    vector3& operator*= (double d){
        e[0] *= d;
        e[1] *= d;
        e[2] *= d;
        return *this;
    }

    double length() const {
        return std::sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
    }

    double length_squared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }
    
    vector3 normalize() const {
        double len = length();
        return vector3(e[0] / len, e[1] / len, e[2] / len);
    }
};

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
inline vector3 operator*(const vector3& v, double d) {
    return d * v;
}

inline vector3 operator/ (const vector3& v, double d) {
    return (1/d) * v;
}

inline double dot(const vector3& v, const vector3& u) {
    return (v.e[0] * u.e[0]) + (v.e[1] * u.e[1]) + (v.e[2] * u.e[2]);
}

inline vector3 cross(const vector3& v, const vector3& u) {
    return vector3(
        v.e[1]*u.e[2] - v.e[2]*u.e[1],
        v.e[2]*u.e[0] - v.e[0]*u.e[2],
        v.e[0]*u.e[1] - v.e[1]*u.e[0]
    );
}

#endif