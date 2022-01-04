#include "renderer.h"

#include <algorithm>

#include "anvil/math/mat4x4f.h"
#include "anvil/math/util.h"
#include "anvil/math/vec4f.h"
#include "anvil/types.h"
#include "util.h"

namespace renderer {

namespace tga = anvil::file::tga;
namespace amath = anvil::math;

void drawLine(tga::Image &image, amath::Vec4f p0, amath::Vec4f p1, tga::Color color) {
    auto pp0 = toFrameCoords(image, p0);
    auto pp1 = toFrameCoords(image, p1);

    bool steep = false;
    if (std::abs(pp0.x - pp1.x) < std::abs(pp0.y - pp1.y)) {  // if the line is steep, we transpose the image
        std::swap(pp0.x, pp0.y);
        std::swap(pp1.x, pp1.y);
        steep = true;
    }
    if (pp0.x > pp1.x) {  // make it left−to−right
        std::swap(pp0.x, pp1.x);
        std::swap(pp0.y, pp1.y);
    }
    int dx = pp1.x - pp0.x;
    int dy = pp1.y - pp0.y;
    // The error variable gives us the distance to the best straight line from our
    // current (x, y) pixel. Each time error is greater than one pixel, we increase
    // (or decrease) y by one, and decrease the error by one as well.
    //float derror = std::abs(dy / float(dx));
    //float error = 0;

    //Why do we need floating points? The only reason is one division by dx and comparison with .5 in the loop body.
    // We can get rid of the floating point by replacing the error variable with another one.
    // Let’s call it error2, and assume it is equal to error * dx * 2
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = pp0.y;
    for (i32_t x = pp0.x; x <= static_cast<i32_t>(pp1.x); x++) {
        if (steep) {
            image.Set(y, image.Height() - x, color);
        } else {
            image.Set(x, image.Height() - y, color);
        }
        error2 += derror2;
        if (error2 > dx) {
            y += (pp1.y > pp0.y ? 1 : -1);
            error2 -= dx * 2;
        }
    }
};

// implements https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLines(tga::Image &image) {
    //
    tga::Color c = tga::Color(1.0, 1.0, 1.0, 1.0);
    auto drawLineV1 = [&image](amath::Vec4f p0, amath::Vec4f p1, tga::Color color) {
        auto pp0 = toFrameCoords(image, p0);
        auto pp1 = toFrameCoords(image, p1);
        for (float t = 0.; t < 1.; t += .001) {
            int x = pp0.x + (pp1.x - pp0.x) * t;
            int y = pp0.y + (pp1.y - pp0.y) * t;
            image.Set(x, image.Height() - y, color);
        }
    };

    auto drawLineV2 = [&image](amath::Vec4f p0, amath::Vec4f p1, tga::Color color) {
        auto pp0 = toFrameCoords(image, p0);
        auto pp1 = toFrameCoords(image, p1);
        for (i32_t x = pp0.x; x <= static_cast<i32_t>(pp1.x); x++) {
            float t = (x - pp0.x) / (float)(pp1.x - pp0.x);
            int y = pp0.y * (1. - t) + pp1.y * t;
            image.Set(x, image.Height() - y, color);
        }
    };

    auto drawLineV3 = [&image](amath::Vec4f p0, amath::Vec4f p1, tga::Color color) {
        auto pp0 = toFrameCoords(image, p0);
        auto pp1 = toFrameCoords(image, p1);

        bool steep = false;
        if (std::abs(pp0.x - pp1.x) < std::abs(pp0.y - pp1.y)) {  // if the line is steep, we transpose the image
            std::swap(pp0.x, pp0.y);
            std::swap(pp1.x, pp1.y);
            steep = true;
        }
        if (pp0.x > pp1.x) {  // make it left−to−right
            std::swap(pp0.x, pp1.x);
            std::swap(pp0.y, pp1.y);
        }
        for (i32_t x = pp0.x; x <= static_cast<i32_t>(pp1.x); x++) {
            float t = (x - pp0.x) / (float)(pp1.x - pp0.x);
            int y = pp0.y * (1. - t) + pp1.y * t;
            if (steep) {
                image.Set(y, image.Height() - x, color);
            } else {
                image.Set(x, image.Height() - y, color);
            }
        }
    };
    (void)(drawLineV1);  // silence unused warning
    (void)(drawLineV2);  // silence unused warning
    (void)(drawLineV3);  // silence unused warning

    drawLine(image, amath::Point(.15, .35, 0), amath::Point(.85, .45, 0), tga::Color(1.0, 1.0, 1.0, 1.0));
    drawLine(image, amath::Point(.35, .15, 0), amath::Point(.45, .85, 0), tga::Color(1.0, 0.0, 0.0, 1.0));
    drawLine(image, amath::Point(.85, .45, 0), amath::Point(.35, .15, 0), tga::Color(1.0, 0.0, 0.0, 1.0));
}

void drawWireframe(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh) {
    for (isize_t fidx = 0; fidx < mesh.faces.Length(); fidx++) {
        for (i64_t eidx = 0; eidx < (i64_t)mesh.faces[fidx].size(); eidx++) {
            auto idx0 = mesh.faces[fidx][eidx];
            auto idx1 = mesh.faces[fidx][(eidx + 1) % 3];
            amath::Vec4f p0 = .5 * mesh.verts[idx0];
            amath::Vec4f p1 = .5 * mesh.verts[idx1];  // idx > 0 ? (idx - 1) : 2

            drawLine(image, p0, p1, tga::Color(1.0, 1.0, 1.0, 1.0));
        }
    }
}

}  // namespace renderer
