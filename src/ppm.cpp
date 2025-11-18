#include "common.hpp"

void write_ppm(const std::string& filename, int width, int height, const std::vector<Vec3>& pixels) {
    std::ofstream out(filename);

    out << "P3\n" << width << " " << height << "\n255\n";

    for (const Vec3& c : pixels) {
        int r = static_cast<int>(255.999 * std::clamp(c.x, 0.0, 1.0));
        int g = static_cast<int>(255.999 * std::clamp(c.y, 0.0, 1.0));
        int b = static_cast<int>(255.999 * std::clamp(c.z, 0.0, 1.0));
        out << r << " " << g << " " << b << "\n";
    }

    out.close();
}
