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

/*
these are naive implementations of line drawing functions

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

*/

// implements https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
void drawLine(tga::Image &image, amath::Vec4f p0, amath::Vec4f p1, tga::Color color) {
    auto pp0 = toFrameCoords(image, p0);
    auto pp1 = toFrameCoords(image, p1);

    bool steep = false;
    // if the line is steep, we transpose the points
    // the line is "steep" if it is taller than it is wide
    if (std::abs(pp0.x - pp1.x) < std::abs(pp0.y - pp1.y)) {
        std::swap(pp0.x, pp0.y);
        std::swap(pp1.x, pp1.y);
        steep = true;
    }
    if (pp0.x > pp1.x) {
        // draw line left−to−right
        std::swap(pp0.x, pp1.x);
        std::swap(pp0.y, pp1.y);
    }
    int dx = pp1.x - pp0.x;
    int dy = pp1.y - pp0.y;
    // The error variable gives us the distance to the best straight line from our
    // current (x, y) pixel. Each time error is greater than one pixel, we increase
    // (or decrease) y by one, and decrease the error by one as well.
    // float derror = std::abs(dy / float(dx));
    // float error = 0;

    // Why do we need floating points? The only reason is one division by dx and comparison with .5 in the loop body.
    //  We can get rid of the floating point by replacing the error variable with another one.
    //  Let’s call it error2, and assume it is equal to error * dx * 2
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

void drawWireframe(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh) {
    for (isize_t fidx = 0; fidx < mesh.faces.Length(); fidx++) {
        for (i64_t eidx = 0; eidx < (i64_t)mesh.faces[fidx].size(); eidx++) {
            auto idx0 = mesh.faces[fidx][eidx].vertex;
            auto idx1 = mesh.faces[fidx][(eidx + 1) % 3].vertex;
            amath::Vec4f p0 = .5 * mesh.verts[idx0];
            amath::Vec4f p1 = .5 * mesh.verts[idx1];  // idx > 0 ? (idx - 1) : 2

            drawLine(image, p0, p1, tga::Color(1.0, 1.0, 1.0, 1.0));
        }
    }
}

// https://github.com/ssloy/tinyrenderer/wiki/Lesson-2:-Triangle-rasterization-and-back-face-culling
// http://www.sunshine2k.de/coding/java/TriangleRasterization/TriangleRasterization.html
void drawTriangle(
    tga::Image &image,
    amath::Vec4f p0,
    amath::Vec4f p1,
    amath::Vec4f p2,
    tga::Color c) {
    //
    if (p0.y > p1.y) std::swap(p0, p1);
    if (p0.y > p2.y) std::swap(p0, p2);
    if (p1.y > p2.y) std::swap(p1, p2);

    f32_t min_x = std::min(p0.x, std::min(p1.x, p2.x));
    f32_t max_x = std::max(p0.x, std::max(p1.x, p2.x));

    f32_t min_y = std::min(p0.y, std::min(p1.y, p2.y));
    f32_t max_y = std::max(p0.y, std::max(p1.y, p2.y));

    for (i32_t x = static_cast<i32_t>(min_x); x <= static_cast<i32_t>(max_x); x++) {
        //
        for (i32_t y = static_cast<i32_t>(min_y); y <= static_cast<i32_t>(max_y); y++) {
            //
            amath::Vec4f p(x, y, 0.0, 0.0);

            amath::Vec4f u = amath::cross(
                amath::Vec4f(p2.x - p0.x, p1.x - p0.x, p0.x - p.x, 0.0),
                amath::Vec4f(p2.y - p0.y, p1.y - p0.y, p0.y - p.y, 0.0));

            auto b = std::abs(u.z) < 1 ? amath::Vec4f(-1.0, 1.0, 1.0, 0.0) : amath::Vec4f(1.f - (u.x + u.y) / u.z, u.y / u.z, u.x / u.z, 0.0);

            if (b.x < 0 || b.y < 0 || b.z < 0) continue;
            image.Set(y, image.Height() - x, c);
        }
    }
}

void drawMesh(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh) {
    for (isize_t fidx = 0; fidx < mesh.faces.Length(); fidx++) {
        //
        const f32_t scale = 0.5;
        assert(mesh.faces[fidx].size() == 3);
        auto vidx0 = mesh.faces[fidx][0].vertex;
        auto vidx1 = mesh.faces[fidx][1].vertex;
        auto vidx2 = mesh.faces[fidx][2].vertex;

        amath::Vec4f v0 = scale * mesh.verts[vidx0];
        amath::Vec4f v1 = scale * mesh.verts[vidx1];
        amath::Vec4f v2 = scale * mesh.verts[vidx2];

        /*

        auto nidx0 = mesh.faces[fidx][0].normal;
        auto nidx1 = mesh.faces[fidx][1].normal;
        auto nidx2 = mesh.faces[fidx][2].normal;

        amath::Vec4f n0 = toFrameCoords(image, mesh.normals[nidx0]);
        amath::Vec4f n1 = toFrameCoords(image, mesh.normals[nidx1]);
        amath::Vec4f n2 = toFrameCoords(image, mesh.normals[nidx2]);
        */

        amath::Vec4f light_dir(0, 0, -1, 0.0);
        amath::Vec4f n = amath::cross((v2 - v0), (v1 - v0));

        auto intensity = amath::dot(amath::norm(n), light_dir);
        auto color = tga::Color(intensity * 1.0, intensity * 1.0, intensity * 1.0, 1.0);
        auto fv0 = toFrameCoords(image, v0);
        auto fv1 = toFrameCoords(image, v1);
        auto fv2 = toFrameCoords(image, v2);
        if (intensity > 0)
            drawTriangle(image, fv0, fv1, fv2, color);
    }
}

// draws three lines to an image
void drawSampleLines(anvil::file::tga::Image &image) {
    //
    tga::Color c = tga::Color(1.0, 1.0, 1.0, 1.0);

    drawLine(image, amath::Point(.15, .35, 0), amath::Point(.85, .45, 0), tga::Color(1.0, 1.0, 1.0, 1.0));
    drawLine(image, amath::Point(.35, .15, 0), amath::Point(.45, .85, 0), tga::Color(1.0, 0.0, 0.0, 1.0));
    drawLine(image, amath::Point(.85, .45, 0), amath::Point(.35, .15, 0), tga::Color(1.0, 0.0, 0.0, 1.0));
}

// draws three triangles to an image
void drawSampleTriangles(anvil::file::tga::Image &image) {
    amath::Vec4f t0[3] = {amath::Point(.1, .4, 0), amath::Point(.5, .6, 0), amath::Point(.4, .8, 0)};
    amath::Vec4f t1[3] = {amath::Point(-.1, -.2, 0), amath::Point(-.125, .3, 0), amath::Point(-.4, -.4, 0)};
    amath::Vec4f t2[3] = {amath::Point(.1, -.3, 0), amath::Point(.425, -.5, 0), amath::Point(.2, -.2, 0)};

    drawTriangle(image,
                 toFrameCoords(image, t0[0]),
                 toFrameCoords(image, t0[1]),
                 toFrameCoords(image, t0[2]),
                 tga::Color(1.0, 0.0, 0.0, 1.0));

    drawTriangle(image,
                 toFrameCoords(image, t1[0]),
                 toFrameCoords(image, t1[1]),
                 toFrameCoords(image, t1[2]),
                 tga::Color(0.0, 1.0, 0.0, 1.0));

    drawTriangle(image,
                 toFrameCoords(image, t2[0]),
                 toFrameCoords(image, t2[1]),
                 toFrameCoords(image, t2[2]),
                 tga::Color(1.0, 1.0, 1.0, 1.0));
}

}  // namespace renderer
