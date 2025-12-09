#pragma once
#include "hittable.hpp"
#include <vector>

struct Plane {
    double A, B, C, D;
};

class Polyhedron : public Hittable {
public:
    std::vector<Plane> faces;
    
    // Indices de material
    int pigmentIndex;
    int finishIndex;

    Polyhedron(const std::vector<Plane>& f, int p_idx, int f_idx) 
        : faces(f), pigmentIndex(p_idx), finishIndex(f_idx) {}

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
};