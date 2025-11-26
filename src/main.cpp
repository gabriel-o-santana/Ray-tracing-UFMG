#include "common.hpp"
#include "scene.hpp" 
#include <iostream>

Vec3 background_color(const Ray& r) {
    return Vec3(0.5, 0.7, 1.0); 
}

Vec3 ray_color(const Ray& r, const Scene& scene) {
    HitRecord rec;
    
    // Se não acertar nada, retorna fundo
    if (!scene.world.hit(r, EPS, 1e9, rec)) {
        return background_color(r);
    }

    // Placeholder visual 
    Vec3 N = rec.normal.normalized();
    return 0.5 * (Vec3(N.x+1, N.y+1, N.z+1)); 
}

int main(int argc, char* argv[]) {
    // Verifica argumentos 
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <input_scene.txt> <output.ppm> [W H]\n";
        return 1;
    }

    const char* inputFile = argv[1];
    const char* outputFile = argv[2];
    
    int W = 800;
    int H = 600; 
    if (argc >= 5) {
        W = std::atoi(argv[3]);
        H = std::atoi(argv[4]);
    }

    // Carregar Cena
    Scene scene;
    if (!scene.load(inputFile)) {
        return 1;
    }

    std::cout << "Cena carregada. Renderizando " << W << "x" << H << "...\n";

    // Setup da Camera (Sistema de coordenadas u,v,w)
    Vec3 lookfrom = scene.camera.eye;
    Vec3 lookat = scene.camera.at;
    Vec3 vup = scene.camera.up;
    double vfov = scene.camera.fovy; 

    // Calculo basico de viewport
    double theta = vfov * 3.1415926535897932385 / 180.0;
    double h = std::tan(theta/2.0);
    double aspect_ratio = (double)W / (double)H;
    double viewport_height = 2.0 * h;
    double viewport_width = aspect_ratio * viewport_height;

    // Vetores da camera
    Vec3 w = (lookfrom - lookat).normalized();
    Vec3 u = (vup.cross(w)).normalized(); 
    Vec3 v = w.cross(u);

    Vec3 horizontal = viewport_width * u;
    Vec3 vertical = viewport_height * v;
    Vec3 lower_left_corner = lookfrom - horizontal/2.0 - vertical/2.0 - w;

    // Render Loop
    std::vector<Vec3> pixels(W * H);
    for (int j = H-1; j >= 0; j--) {
        for (int i = 0; i < W; i++) {
            double s = (double)i / (W-1);
            double t = (double)j / (H-1);
            
            Ray r(lookfrom, lower_left_corner + s*horizontal + t*vertical - lookfrom);
            pixels[(H-1-j)*W + i] = ray_color(r, scene);
        }
    }

    write_ppm(outputFile, W, H, pixels);
    std::cout << "Concluido: " << outputFile << "\n";
    return 0;
}