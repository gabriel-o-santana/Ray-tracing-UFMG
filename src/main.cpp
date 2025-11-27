#include "common.hpp"
#include "scene.hpp"
#include <iostream>
#include <cmath>

Vec3 get_pigment_color(const Pigment& pig, const Vec3& p) {
    if (pig.type == SOLID) {
        return pig.color1;
    }
    else if (pig.type == CHECKER) {
        int cx = (int)floor(p.x / pig.cube_size);
        int cy = (int)floor(p.y / pig.cube_size);
        int cz = (int)floor(p.z / pig.cube_size);
        if ((std::abs(cx + cy + cz) % 2) == 0) return pig.color1;
        else return pig.color2;
    }
    return Vec3(1, 0, 1);
}

bool in_shadow(const Scene& scene, const Vec3& point, const Vec3& lightPos) {
    Vec3 toLight = lightPos - point;
    double dist = std::sqrt(toLight.dot(toLight));
    Vec3 dir = toLight.normalized();
    Ray shadowRay(point + dir * 1e-4, dir);
    HitRecord rec;
    if (scene.world.hit(shadowRay, 0.001, dist, rec)) {
        return true;
    }
    return false;
}

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * v.dot(n) * n;
}

// Implementacao da Lei de Snell para refracao
Vec3 refract(const Vec3& uv, const Vec3& n, double etai_over_etat) {
    double cos_theta = std::min((-1.0 * uv).dot(n), 1.0);
    Vec3 r_out_perp =  etai_over_etat * (uv + cos_theta*n);
    double r_out_parallel_sq = 1.0 - r_out_perp.dot(r_out_perp);
    
    // Se for negativo, ocorreu Reflexao Interna Total (tratado no caller)
    if (r_out_parallel_sq < 0) return Vec3(0,0,0);
    
    Vec3 r_out_parallel = -std::sqrt(std::fabs(r_out_parallel_sq)) * n;
    return r_out_perp + r_out_parallel;
}

Vec3 ray_color(const Ray& r, const Scene& scene, int depth) {
    if (depth <= 0) return Vec3(0,0,0);

    HitRecord rec;
    if (!scene.world.hit(r, 0.001, 1e9, rec)) {
        return Vec3(0.5, 0.7, 1.0); 
    }

    const Pigment& pig = scene.pigments[rec.pigmentIndex];
    const Finish& fin = scene.finishes[rec.finishIndex];
    
    Vec3 surfaceColor = get_pigment_color(pig, rec.point);
    Vec3 N = rec.normal.normalized();
    Vec3 V = r.direction.normalized(); 

    Vec3 localColor = surfaceColor * fin.ka; 

    // Iluminacao Direta (Phong) 
    for (const auto& light : scene.lights) {
        Vec3 L_vec = light.position - rec.point;
        double d = std::sqrt(L_vec.dot(L_vec));
        Vec3 L = L_vec.normalized();

        if (in_shadow(scene, rec.point, light.position)) continue;

        double attenuation = 1.0 / (light.att_const + light.att_linear * d + light.att_quad * d * d);
        
        double diff = std::max(0.0, N.dot(L));
        Vec3 diffuse = surfaceColor * light.color * (fin.kd * diff);

        Vec3 R_light = reflect(-1.0 * L, N); 
        double spec = std::pow(std::max(0.0, R_light.dot(-1.0*V)), fin.alpha);
        Vec3 specular = light.color * (fin.ks * spec);

        localColor = localColor + (diffuse + specular) * attenuation;
    }

    //  Reflexão (Mirror)
    if (fin.kr > 0) {
        Vec3 reflectDir = reflect(V, N).normalized();
        Ray scattered(rec.point + reflectDir * 1e-4, reflectDir);
        localColor = localColor + ray_color(scattered, scene, depth - 1) * fin.kr;
    }

    // Refração (Vidro/Agua) 
    if (fin.kt > 0) {
        bool entering = V.dot(N) < 0;

        double refraction_ratio = entering ? (1.0 / fin.ior) : fin.ior;

        Vec3 unit_n = entering ? N : -1.0 * N;
        
        double cos_theta = std::min((-1.0 * V).dot(unit_n), 1.0);
        double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;

        Vec3 direction;
        if (cannot_refract) {
            direction = reflect(V, unit_n);
        } else {
            direction = refract(V, unit_n, refraction_ratio);
        }

        Ray scattered(rec.point + direction * 1e-4, direction);
        localColor = localColor + ray_color(scattered, scene, depth - 1) * fin.kt;
    }

    return localColor;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <input.txt> <output.ppm> [W H]\n";
        return 1;
    }

    Scene scene;
    if (!scene.load(argv[1])) return 1;

    int W = 800, H = 600;
    if (argc >= 5) { W = std::atoi(argv[3]); H = std::atoi(argv[4]); }

    std::cout << "Renderizando " << W << "x" << H << " com refracao...\n";

    Vec3 w = (scene.camera.eye - scene.camera.at).normalized();
    Vec3 u = (scene.camera.up.cross(w)).normalized();
    Vec3 v = w.cross(u);

    double theta = scene.camera.fovy * 3.141592653589 / 180.0;
    double h = tan(theta/2);
    double viewport_height = 2.0 * h;
    double viewport_width = viewport_height * ((double)W/H);

    Vec3 horizontal = viewport_width * u;
    Vec3 vertical = viewport_height * v;
    Vec3 lower_left = scene.camera.eye - horizontal/2 - vertical/2 - w;

    std::vector<Vec3> pixels(W * H);
    int max_depth = 5;

    for (int j = H-1; j >= 0; j--) {
        for (int i = 0; i < W; i++) {
            double s = (double)i / (W-1);
            double t = (double)j / (H-1);
            Ray r(scene.camera.eye, lower_left + s*horizontal + t*vertical - scene.camera.eye);
            pixels[(H-1-j)*W + i] = ray_color(r, scene, max_depth);
        }
    }

    write_ppm(argv[2], W, H, pixels);
    return 0;
}