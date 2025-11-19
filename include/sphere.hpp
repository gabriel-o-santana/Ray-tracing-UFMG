#pragma once
#include "hittable.hpp"

class Sphere : public Hittable {
public:
    Vec3 center;
    double radius;

    Sphere() {
        center = Vec3();
        radius = 1.0;
    }
    
    Sphere(const Vec3& c, double r) {
        center = c;
        radius = r;
    }

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
};