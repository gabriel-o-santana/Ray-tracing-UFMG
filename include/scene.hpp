#pragma once
#include "common.hpp"
#include "hittable.hpp"
#include "sphere.hpp"
#include "polyhedron.hpp" // Novo include
#include "texture.hpp"    // Novo include
#include <vector>
#include <string>
#include <memory>

enum PigmentType { SOLID, CHECKER, TEXMAP };

struct Pigment {
    PigmentType type;
    Vec3 color1; 
    Vec3 color2; 
    double cube_size; 

    // Dados para Texture Map
    std::shared_ptr<Texture> texture; 
    // Coeficientes dos planos de projecao (s = P0.dot(v), r = P1.dot(v))
    // Armazenando como 4 doubles: x, y, z, w (offset)
    double p0[4]; 
    double p1[4];
};

struct Finish {
    double ka, kd, ks, alpha; 
    double kr, kt, ior;      
};

struct Camera {
    Vec3 eye, at, up;
    double fovy; 
};

class Scene {
public:
    Camera camera;
    std::vector<Light> lights;
    std::vector<Pigment> pigments;
    std::vector<Finish> finishes;
    HittableList world;

    bool load(const std::string& filename);
};