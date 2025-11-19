#pragma once
#include "common.hpp"
#include <vector>

struct HitRecord {
    double t;
    Vec3 point;
    Vec3 normal;
};

// Interface para objetos atingíveis por um raio
class Hittable {
public:
    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const = 0;
    virtual ~Hittable() {}
};

// Lista de objetos da cena
class HittableList : public Hittable {
public:
    std::vector<Hittable*> objects;

    void add(Hittable* obj);

    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
};
