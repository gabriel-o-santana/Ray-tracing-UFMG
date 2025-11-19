#include "common.hpp"

Vec3 ray_color(const Ray& r) {
    Sphere sphere(Vec3(0, 0, -1), 0.5);

    double t;
    if (hit_sphere(sphere, r, t)) {
        Vec3 point = r.at(t);
        Vec3 normal = (point - sphere.center).normalized();
        return Vec3(normal.x + 1, normal.y + 1, normal.z + 1) * 0.5; // mapa normal -> cor
    }

    // Fundo degradê (céu)
    Vec3 unit_dir = r.direction.normalized();
    double u = 0.5 * (unit_dir.y + 1.0);
    return Vec3(1.0, 1.0, 1.0) * (1.0 - u) + Vec3(0.5, 0.7, 1.0) * u;
}

int main() {
    const int WIDTH = 400;
    const int HEIGHT = 300;

    Vec3 camera_origin(0, 0, 0);

    std::vector<Vec3> pixels(WIDTH * HEIGHT);

    double viewport_height = 2.0;
    double viewport_width = viewport_height * (double(WIDTH) / HEIGHT);
    double focal_length = 1.0;

    Vec3 horizontal(viewport_width, 0, 0);
    Vec3 vertical(0, viewport_height, 0);

    Vec3 lower_left = camera_origin 
        - horizontal * 0.5 
        - vertical * 0.5 
        - Vec3(0, 0, focal_length);

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            double u = double(i) / (WIDTH - 1);
            double v = double(j) / (HEIGHT - 1);

            Ray r(camera_origin, (lower_left + horizontal*u + vertical*v) - camera_origin);

            pixels[j * WIDTH + i] = ray_color(r);
        }
    }

    write_ppm("sphere.ppm", WIDTH, HEIGHT, pixels);

    std::cout << "Imagem gerada: sphere.ppm" << std::endl;
    return 0;
}
