#include "../include/polyhedron.hpp"
#include <limits>
#include <cmath>

bool Polyhedron::hit(const Ray& r, double tmin, double tmax, HitRecord& rec) const {
    double t_near = -std::numeric_limits<double>::infinity();
    double t_far = std::numeric_limits<double>::infinity();
    int hit_plane_index = -1;

    // Algoritmo de interseção Slab (interseção de semi-espaços)
    for (int i = 0; i < (int)faces.size(); i++) {
        const Plane& p = faces[i];
        
        // Normal do plano (A, B, C)
        Vec3 n(p.A, p.B, p.C);
        
        // Denominador: N . Dir
        double vn = n.dot(r.direction);
        
        // Numerador: -(N . Origin + D)
        double vd = -(n.dot(r.origin) + p.D);

        if (std::abs(vn) < 1e-6) {
            // Raio paralelo ao plano. Se fora do semi-espaço, errou o poliedro.
            if (vd < 0) return false; 
        } else {
            double t = vd / vn;
            if (vn < 0) { 
                // Entrando no semi-espaço
                if (t > t_near) {
                    t_near = t;
                    hit_plane_index = i;
                }
            } else { 
                // Saindo do semi-espaço
                if (t < t_far) {
                    t_far = t;
                }
            }
        }
    }

    if (t_near > t_far || t_far < 0) return false;

    // Verifica se a interseção está dentro dos limites válidos
    double t_hit = t_near;
    if (t_hit < tmin) {
        t_hit = t_far; 
        if (t_hit < tmin) return false;
    }
    if (t_hit > tmax) return false;

    rec.t = t_hit;
    rec.point = r.at(rec.t);
    
    // Normal é a normal da face atingida
    if (hit_plane_index != -1) {
        const Plane& p = faces[hit_plane_index];
        rec.normal = Vec3(p.A, p.B, p.C).normalized();
    } else {
        // Caso atingiu a face "de tras" (dentro do objeto), inverte normal
        // Simplificacao: recalcula o mais proximo
        rec.normal = Vec3(0,1,0); 
    }

    rec.pigmentIndex = this->pigmentIndex;
    rec.finishIndex = this->finishIndex;

    return true;
}