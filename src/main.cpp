#include "common.hpp"

int main() {
    const int WIDTH = 400;
    const int HEIGHT = 300;

    std::vector<Vec3> pixels(WIDTH * HEIGHT);

    // Geração do gradiente
    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            double r = double(i) / (WIDTH - 1);
            double g = double(j) / (HEIGHT - 1);
            double b = 0.2;

            pixels[j * WIDTH + i] = Vec3(r, g, b);
        }
    }

    write_ppm("output.ppm", WIDTH, HEIGHT, pixels);

    std::cout << "Imagem gerada: output.ppm" << std::endl;

    return 0;
}
