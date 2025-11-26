#pragma once
#include "hittable.hpp"

class Sphere : public Hittable {
public:
    Vec3 center;
    double radius;
    
    // Referencias para materiais 
    int pigmentIndex;
    int finishIndex;

    Sphere() : center(Vec3()), radius(1.0), pigmentIndex(0), finishIndex(0) {}
    
    Sphere(const Vec3& c, double r, int p_idx, int f_idx) 
        : center(c), radius(r), pigmentIndex(p_idx), finishIndex(f_idx) {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
};