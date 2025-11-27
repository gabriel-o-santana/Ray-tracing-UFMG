#pragma once
#include "common.hpp"
#include "sphere.hpp"
#include <vector>
#include <cmath>

bool Sphere::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    Vec3 oc = r.origin - center;
    double a = r.direction.dot(r.direction);
    double half_b = oc.dot(r.direction);
    double c = oc.dot(oc) - radius * radius;
    double discriminant = half_b * half_b - a * c;

    if (discriminant < 0) return false;
    double sqrtd = std::sqrt(discriminant);

    // Encontra a raiz mais proxima dentro do intervalo
    double root = (-half_b - sqrtd) / a;
    if (root < tmin || root > tmax) {
        root = (-half_b + sqrtd) / a;
        if (root < tmin || root > tmax)
            return false;
    }

    rec.t = root;
    rec.point = r.at(rec.t);
    Vec3 outward_normal = (rec.point - center) / radius;
    rec.normal = outward_normal; 

    rec.pigmentIndex = this->pigmentIndex;
    rec.finishIndex = this->finishIndex;

    return true;
}