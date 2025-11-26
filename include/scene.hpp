#pragma once
#include "common.hpp"
#include "hittable.hpp"
#include "sphere.hpp"
#include <vector>
#include <string>

// Tipos de pigmento
enum PigmentType { SOLID, CHECKER, TEXMAP };

struct Pigment {
    PigmentType type;
    Vec3 color1; 
    Vec3 color2; 
    double cube_size; 
};

// Coeficientes de acabamento
struct Finish {
    double ka, kd, ks, alpha; 
    double kr, kt, ior;      
};

struct Camera {
    Vec3 eye;
    Vec3 at;
    Vec3 up;
    double fovy; 
};

class Scene {
public:
    Camera camera;
    std::vector<Light> lights;
    std::vector<Pigment> pigments;
    std::vector<Finish> finishes;
    
    HittableList world; // Lista de objetos (Esferas, Poliedros)

    // Carrega o arquivo de cena
    bool load(const std::string& filename);
};