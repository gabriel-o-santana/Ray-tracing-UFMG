#include "../include/scene.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

bool Scene::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return false;
    }

    // Camera 
    file >> camera.eye.x >> camera.eye.y >> camera.eye.z;
    file >> camera.at.x >> camera.at.y >> camera.at.z;
    file >> camera.up.x >> camera.up.y >> camera.up.z;
    file >> camera.fovy;

    // Luzes 
    int numLights;
    file >> numLights;
    for (int i = 0; i < numLights; i++) {
        Light l;
        // grandezas: pos(3), cor(3), att(3)
        file >> l.position.x >> l.position.y >> l.position.z;
        file >> l.color.x >> l.color.y >> l.color.z;
        file >> l.att_const >> l.att_linear >> l.att_quad;
        lights.push_back(l);
    }

    // Pigmentos 
    int numPigments;
    file >> numPigments;
    for (int i = 0; i < numPigments; i++) {
        Pigment p;
        std::string typeStr;
        file >> typeStr; // "solid", "checker", etc.

        if (typeStr == "solid") {
            p.type = SOLID;
            file >> p.color1.x >> p.color1.y >> p.color1.z;
        } 
        else if (typeStr == "checker") {
            p.type = CHECKER;
            file >> p.color1.x >> p.color1.y >> p.color1.z;
            file >> p.color2.x >> p.color2.y >> p.color2.z;
            file >> p.cube_size;
        }
        else if (typeStr == "texmap") {
            p.type = TEXMAP;


            std::string texFile;
            file >> texFile; 

            double dummy;
            for(int k=0; k<8; k++) file >> dummy;
        }
        pigments.push_back(p);
    }

    // Acabamentos 
    int numFinishes;
    file >> numFinishes;
    for (int i = 0; i < numFinishes; i++) {
        Finish f;
        file >> f.ka >> f.kd >> f.ks >> f.alpha >> f.kr >> f.kt >> f.ior;
        finishes.push_back(f);
    }

    // Objetos 
    int numObjects;
    file >> numObjects;
    for (int i = 0; i < numObjects; i++) {
        int pigIdx, finIdx;
        std::string objType;
        
        file >> pigIdx >> finIdx >> objType;

        if (objType == "sphere") { 
            double x, y, z, r;
            file >> x >> y >> z >> r;
            // Cria a esfera e adiciona ao mundo
            Sphere* s = new Sphere(Vec3(x,y,z), r, pigIdx, finIdx);
            world.add(s);
        }
        else if (objType == "polyhedron") { 
            int numFaces;
            file >> numFaces;
            // Consumir as linhas das faces para não quebrar o parser
            // (Implementação do poliedro fica para depois)
            double a, b, c, d;
            for(int k=0; k<numFaces; k++) {
                file >> a >> b >> c >> d;
            }
            std::cout << "Aviso: Polyhedron lido mas ignorado.\n";
        }
    }

    return true;
}