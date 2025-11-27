#pragma once
#include "common.hpp"
#include <vector>

struct HitRecord {
    double t;
    Vec3 point;
    Vec3 normal;
    // Novos campos para saber o material do objeto atingido
    int pigmentIndex;
    int finishIndex;
};

class Hittable {
public:
    virtual bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const = 0;
    virtual ~Hittable() {}
};

class HittableList : public Hittable {
public:
    std::vector<Hittable*> objects;

    void add(Hittable* obj); // Implementado em hittable.cpp
    bool hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const override;
};