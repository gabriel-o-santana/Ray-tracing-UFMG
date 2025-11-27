#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <vector>
#include <limits>

struct Vec3
{
    double x, y, z;

    Vec3() {
        x = 0;
        y = 0;
        z = 0;
    }
    Vec3(double X, double Y, double Z) {
        x = X;
        y = Y;
        z = Z;
    }

    Vec3 operator+(const Vec3 &o) const { 
        return Vec3(x + o.x, y + o.y, z + o.z); 
    }
    Vec3 operator-(const Vec3 &o) const { 
        return Vec3(x - o.x, y - o.y, z - o.z); 
    }
    
    Vec3 operator*(double k) const { 
        return Vec3(x * k, y * k, z * k); 
    }
    
    Vec3 operator*(const Vec3 &o) const {
        return Vec3(x * o.x, y * o.y, z * o.z);
    }

    Vec3 operator/(double k) const { 
        return Vec3(x / k, y / k, z / k); 
    }

    Vec3 cross(const Vec3 &o) const {
        return Vec3(y * o.z - z * o.y,
                    z * o.x - x * o.z,
                    x * o.y - y * o.x);
    }

    double dot(const Vec3 &o) const { 
        return x * o.x + y * o.y + z * o.z; 
    }

    Vec3 normalized() const
    {
        double m = std::sqrt(x * x + y * y + z * z);
        if (m == 0) return Vec3(0,0,0);
        return Vec3(x / m, y / m, z / m);
    }
};

// Multiplicação inversa (numero * vetor)
inline Vec3 operator*(double k, const Vec3 &v) { 
    return v * k; 
}

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray() {}
    Ray(const Vec3 &o, const Vec3 &d) {
        origin = o;
        direction = d;
    }

    Vec3 at(double t) const
    {
        return origin + direction * t;
    }
};

struct Light {
    Vec3 position;  
    Vec3 color;     
    
    // Atenuação
    double att_const;
    double att_linear;
    double att_quad;
};

// Lambert simples (mantido para compatibilidade, mas não usado no main atual)
Vec3 lambert(const Vec3 &normal, const Light &light);

// Gerar arquivo PPM
void write_ppm(const std::string &filename, int W, int H, const std::vector<Vec3> &pixels);

static constexpr double EPS = 1e-6;