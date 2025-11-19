#include "sphere.hpp"
#include <cmath>

bool Sphere::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    Vec3 oc;
    oc.x = r.origin.x - center.x;
    oc.y = r.origin.y - center.y;
    oc.z = r.origin.z - center.z;

    double a = r.direction.dot(r.direction);
    double b = oc.dot(r.direction);
    double c = oc.dot(oc) - radius * radius;

    double discriminant = b*b - a*c;
    if (discriminant < 0.0) return false;

    double sq = std::sqrt(discriminant);

    double root = (-b - sq) / a;
    if (root < tmin || root > tmax) {
        root = (-b + sq) / a;
        if (root < tmin || root > tmax) return false;
    }

    rec.t = root;
    rec.point = r.at(root);

    Vec3 outward;
    outward.x = (rec.point.x - center.x) / radius;
    outward.y = (rec.point.y - center.y) / radius;
    outward.z = (rec.point.z - center.z) / radius;

    rec.normal = outward;
    return true;
}
