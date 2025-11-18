#include "common.hpp"

int main() {
    Vec3 a(1, 2, 3);
    Vec3 b(4, 5, 6);
    Vec3 c = a + b;

    std::cout << "Ray Tracer ambiente configurado!" << std::endl;
    std::cout << "Teste vector: (" << c.x << ", " << c.y << ", " << c.z << ")" << std::endl;
    return 0;
}
