#pragma once
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>

struct Vec3
{
    double x, y, z;

    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3 &v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3 &v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double s) const { return Vec3(x * s, y * s, z * s); }

    Vec3 normalized() const
    {
        double m = std::sqrt(x * x + y * y + z * z);
        return Vec3(x / m, y / m, z / m);
    }
};

struct Ray
{
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3 &o, const Vec3 &d) : origin(o), direction(d) {}

    Vec3 at(double t) const
    {
        return origin + direction * t;
    }
};

struct Sphere
{
    Vec3 center;
    double radius;

    Sphere(const Vec3 &c, double r) : center(c), radius(r) {}
};

void write_ppm(const std::string &filename, int width, int height, const std::vector<Vec3> &pixels);

bool hit_sphere(const Sphere &sphere, const Ray &ray, double &t_hit);
