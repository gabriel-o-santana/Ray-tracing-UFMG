#include "common.hpp"
#include <fstream>

Vec3 lambert(const Vec3& normal, const Light& light) {

    Vec3 L = Vec3(-light.direction.x, -light.direction.y, -light.direction.z);
    // normalize L and normal
    Vec3 Ln = L.normalized();
    Vec3 Nn = normal.normalized();

    double diff = Nn.dot(Ln);
    if (diff < 0.0) diff = 0.0;

    // retorno intensidade da luz (multiplicada pela cor da luz)
    Vec3 out;
    out.x = light.color.x * diff;
    out.y = light.color.y * diff;
    out.z = light.color.z * diff;
    return out;
}

void write_ppm(const std::string& filename, int W, int H, const std::vector<Vec3>& pixels) {
    std::ofstream out(filename);

    out << "P3\n";
    out << W << " " << H << "\n255\n";

    for (int i = 0; i < (int)pixels.size(); i++) {
        double rx = pixels[i].x;
        double gx = pixels[i].y;
        double bx = pixels[i].z;

        // clamp to [0,1]
        if (rx < 0.0) rx = 0.0; if (rx > 1.0) rx = 1.0;
        if (gx < 0.0) gx = 0.0; if (gx > 1.0) gx = 1.0;
        if (bx < 0.0) bx = 0.0; if (bx > 1.0) bx = 1.0;

        int r = (int)(255.999 * rx);
        int g = (int)(255.999 * gx);
        int b = (int)(255.999 * bx);

        out << r << " " << g << " " << b << "\n";
    }

    out.close();
}
