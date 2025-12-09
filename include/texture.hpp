#pragma once
#include "common.hpp"
#include <vector>
#include <string>
#include <fstream>

struct Texture {
    int width, height;
    std::vector<Vec3> pixels;

    bool load(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string format;
        file >> format;
        if (format != "P3") return false; 

        int maxVal;
        file >> width >> height >> maxVal;

        pixels.resize(width * height);
        for (int i = 0; i < width * height; ++i) {
            int r, g, b;
            file >> r >> g >> b;
            pixels[i] = Vec3(r / 255.0, g / 255.0, b / 255.0);
        }
        return true;
    }

    Vec3 sample(double u, double v) const {
        if (pixels.empty()) return Vec3(1, 0, 1);

        // Repeticao da textura
        u = u - floor(u);
        v = v - floor(v);

        int i = static_cast<int>(u * width);
        int j = static_cast<int>(v * height);

        if (i < 0) i = 0; if (i >= width) i = width - 1;
        if (j < 0) j = 0; if (j >= height) j = height - 1;

        return pixels[j * width + i]; // PPM armazena linha a linha
    }
};