#include "common.hpp"
#include "scene.hpp"
#include <iostream>
#include <cmath>

// Resolve a cor do ponto baseado no tipo de pigmento (Solid ou Checker)
Vec3 get_pigment_color(const Pigment& pig, const Vec3& p) {
    if (pig.type == SOLID) {
        return pig.color1;
    }
    else if (pig.type == CHECKER) {
        // Logica do tabuleiro 3D: soma das coordenadas inteiras do cubo
        int cx = (int)floor(p.x / pig.cube_size);
        int cy = (int)floor(p.y / pig.cube_size);
        int cz = (int)floor(p.z / pig.cube_size);
        
        // Se a soma for par usa cor1, impar usa cor2
        if ((std::abs(cx + cy + cz) % 2) == 0) return pig.color1;
        else return pig.color2;
    }
    return Vec3(1, 0, 1); // Erro (Magenta)
}

// Verifica se um ponto esta na sombra de uma luz especifica
bool in_shadow(const Scene& scene, const Vec3& point, const Vec3& lightPos) {
    Vec3 toLight = lightPos - point;
    double dist = std::sqrt(toLight.dot(toLight));
    Vec3 dir = toLight.normalized();

    Ray shadowRay(point + dir * 1e-4, dir); // Offset para evitar acne de sombra
    HitRecord rec;
    
    // Se bater em algo antes de chegar na luz (dist), esta na sombra
    if (scene.world.hit(shadowRay, 0.001, dist, rec)) {
        return true;
    }
    return false;
}

Vec3 ray_color(const Ray& r, const Scene& scene) {
    HitRecord rec;
    
    // Intersecao com o mundo
    if (!scene.world.hit(r, 0.001, 1e9, rec)) {
        return Vec3(0.5, 0.7, 1.0); 
    }

    // Busca dados do material
    const Pigment& pig = scene.pigments[rec.pigmentIndex];
    const Finish& fin = scene.finishes[rec.finishIndex];
    
    Vec3 surfaceColor = get_pigment_color(pig, rec.point);
    Vec3 N = rec.normal.normalized();
    Vec3 V = (r.origin - rec.point).normalized(); // Vetor vista

    Vec3 finalColor = surfaceColor * fin.ka; 

    // Loop de Luzes
    for (const auto& light : scene.lights) {
        Vec3 L_vec = light.position - rec.point;
        double d = std::sqrt(L_vec.dot(L_vec)); // Distancia da luz
        Vec3 L = L_vec.normalized();

        // Teste de Sombra
        if (in_shadow(scene, rec.point, light.position)) {
            continue; // Se estiver na sombra, luz nao contribui (apenas ambiente)
        }

        // Atenuacao 
        double attenuation = 1.0 / (light.att_const + light.att_linear * d + light.att_quad * d * d);
        
        // Difusa (Lambert)
        double diffFactor = std::max(0.0, N.dot(L));
        Vec3 diffuse = surfaceColor * light.color * (fin.kd * diffFactor);

        // Especular (Phong) 
        Vec3 R = (2.0 * N.dot(L) * N - L).normalized();
        double specFactor = std::pow(std::max(0.0, R.dot(V)), fin.alpha);
        Vec3 specular = light.color * (fin.ks * specFactor);

        // Soma tudo ponderado pela atenuacao
        finalColor = finalColor + (diffuse + specular) * attenuation;
    }

    return finalColor;
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

    std::cout << "Renderizando " << W << "x" << H << "...\n";

    // Configuracao da camera
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

    for (int j = H-1; j >= 0; j--) {
        for (int i = 0; i < W; i++) {
            double s = (double)i / (W-1);
            double t = (double)j / (H-1);
            Ray r(scene.camera.eye, lower_left + s*horizontal + t*vertical - scene.camera.eye);
            pixels[(H-1-j)*W + i] = ray_color(r, scene);
        }
    }

    write_ppm(argv[2], W, H, pixels);
    return 0;
}