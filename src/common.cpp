#include "common.hpp"
#include <cmath>

bool hit_sphere(const Sphere &sphere, const Ray &ray, double &t_hit)
{
    Vec3 oc = ray.origin - sphere.center;
    double a = ray.direction.x * ray.direction.x + ray.direction.y * ray.direction.y + ray.direction.z * ray.direction.z;
    double b = 2.0 * (oc.x * ray.direction.x + oc.y * ray.direction.y + oc.z * ray.direction.z);
    double c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - sphere.radius * sphere.radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant < 0)
        return false;

    t_hit = (-b - std::sqrt(discriminant)) / (2 * a);
    return t_hit > 0;
}
