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

Vec3 lambert(const Vec3 &normal, const Light &light)
{
    double ndotl = std::max(0.0, normal.normalized().dot(light.direction.normalized() * -1));
    return light.color * ndotl;
}

bool is_in_shadow(const Vec3 &point, const Light &light, const Sphere &sphere)
{
    Vec3 L = light.direction.normalized() * -1.0;

    // ensure Vec3 * double is used (member operator), not double * Vec3 which isn't defined
    Ray shadow_ray(point + L * (sphere.radius * 0.001), L);

    double t_shadow;
    if (hit_sphere(sphere, shadow_ray, t_shadow))
    {
        if (t_shadow > EPS)
        {
            return true;
        }
    }
    return false;
}