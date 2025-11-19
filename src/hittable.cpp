#include "../include/hittable.hpp"

void HittableList::add(Hittable* obj) {
    objects.push_back(obj);
}

bool HittableList::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    HitRecord temp;
    bool hitSomething = false;
    double closest = tmax;

    for (int i = 0; i < (int)objects.size(); i++) {
        if (objects[i]->hit(r, tmin, closest, temp)) {
            hitSomething = true;
            closest = temp.t;
            rec = temp;
        }
    }
    return hitSomething;
}
