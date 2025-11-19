#include "common.hpp"
#include "hittable.hpp"
#include "sphere.hpp"
#include <vector>

Vec3 background_color(const Ray& r) {
    Vec3 d = r.direction.normalized();
    double t = 0.5 * (d.y + 1.0);
    // return white -> bluish gradient
    return Vec3(1.0*(1.0 - t) + 0.5 * t, 1.0*(1.0 - t) + 0.7 * t, 1.0*(1.0 - t) + 1.0 * t);
}

// shadow test (directional light). Returns true if point is in shadow
bool in_shadow(const Vec3& point, const Light& light, const HittableList& world) {
    // compute vector from point TO the light: L = -light.direction
    Vec3 L;
    L.x = -light.direction.x;
    L.y = -light.direction.y;
    L.z = -light.direction.z;

    // normalize
    L = L.normalized();

    // offset origin a little along L to avoid self intersection
    Vec3 origin = point + L * (EPS * 100.0);

    Ray shadowRay(origin, L);

    HitRecord tmp;
    // directional light is at infinity -> tmax can be large
    if (world.hit(shadowRay, EPS, 1e6, tmp)) {
        return true;
    }
    return false;
}

Vec3 ray_color(const Ray& r, const HittableList& world, const Light& light) {
    HitRecord rec;

    if (!world.hit(r, EPS, 1e9, rec)) {
        return background_color(r);
    }

    Vec3 normal = rec.normal.normalized();

    // ambient term small so shadowed areas are not completely black
    Vec3 ambient = Vec3(0.08, 0.08, 0.08);

    if (in_shadow(rec.point, light, world)) {
        return ambient;
    }

    Vec3 diffuse = lambert(normal, light);

    // combine ambient + diffuse
    Vec3 color;
    color.x = ambient.x + diffuse.x;
    color.y = ambient.y + diffuse.y;
    color.z = ambient.z + diffuse.z;

    return color;
}

int main() {
    int W = 800;
    int H = 450;

    std::vector<Vec3> pixels;
    pixels.resize(W * H);

    Vec3 origin;
    origin.x = 0.0; origin.y = 0.0; origin.z = 0.0;

    HittableList world;


    Sphere* s1 = new Sphere(Vec3(0, 0, -3), 1.0);
    Sphere* ground = new Sphere(Vec3(0, -1001, -3), 1000.0);

    world.add(s1);

    world.add(ground);

    Light light;

    
    light.direction.x = -1.0; light.direction.y = -1.0; light.direction.z = -1.0;
    light.direction = light.direction.normalized();
    light.color.x = 1.0; light.color.y = 1.0; light.color.z = 1.0;

    double viewport_h = 2.0;
    double viewport_w = (double)W / (double)H * viewport_h;
    double focal_len = 1.0;

    Vec3 horizontal;
    horizontal.x = viewport_w; horizontal.y = 0.0; horizontal.z = 0.0;
    Vec3 vertical;
    vertical.x = 0.0; vertical.y = viewport_h; vertical.z = 0.0;

    Vec3 lower_left;
    lower_left.x = origin.x - horizontal.x/2.0 - vertical.x/2.0 - 0.0;
    lower_left.y = origin.y - horizontal.y/2.0 - vertical.y/2.0 - 0.0;
    lower_left.z = origin.z - horizontal.z/2.0 - vertical.z/2.0 - focal_len;

    for (int j = H-1; j >= 0; j--) {
        for (int i = 0; i < W; i++) {
            double u = (double)i / (double)(W - 1);
            double v = (double)j / (double)(H - 1);

            Vec3 dir;
            dir.x = lower_left.x + horizontal.x * u + vertical.x * v - origin.x;
            dir.y = lower_left.y + horizontal.y * u + vertical.y * v - origin.y;
            dir.z = lower_left.z + horizontal.z * u + vertical.z * v - origin.z;

            Ray r(origin, dir);

            Vec3 color = ray_color(r, world, light);


            int row = H - 1 - j; 
            pixels[row * W + i] = color;
        }
    }

    write_ppm("output.ppm", W, H, pixels);

    return 0;
}
