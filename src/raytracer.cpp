#include "raytracer.h"

#include <iostream>

#include "anvil/file/tga.h"
#include "anvil/math/mat4x4f.h"
#include "anvil/math/util.h"
#include "anvil/math/vec4f.h"
#include "util.h"

namespace raytracer {

namespace tga = anvil::file::tga;
namespace amath = anvil::math;

void drawLine(tga::Image &image) {
    for (isize_t idx = 0; idx < image.Width(); idx++) {
        image.Set(idx, (image.Height() / 2) - 1, tga::Color(1.0, 0, 0, 1.0));
        image.Set(idx, image.Height() / 2, tga::Color(0, 1.0, 0, 1.0));
        image.Set(idx, (image.Height() / 2) + 1, tga::Color(0, 0, 1.0, 1.0));
    }
}

void drawProjectile(tga::Image &image) {
    amath::Vec4f p(0.0, 1.0, 0.0, 1.0);
    amath::Vec4f v(1.0, 1.8, 0.0, 0.0);
    v = amath::norm(v) * 11.25;
    amath::Vec4f g(0.0, -0.1, 0.0, 0.0);
    amath::Vec4f w(-0.01, 0.0, 0.0, 0.0);

    for (isize_t idx = 0; idx < image.Width(); idx++) {
        p = p + v;
        v = v + g + w;
        image.Set(p.x, image.Height() - p.y, tga::Color(1.0, 0.0, 0.0, 1.0));
    }
}

void drawClockFace(tga::Image &image) {
    tga::Color c = tga::Color(1.0, 1.0, 1.0, 1.0);

    auto drawPoint = [&image](amath::Vec4f p, isize_t size, tga::Color c) {
        //x: -1 == 0, 1 == width, 0 == width / 2
        //y: -1 == 0, 1 == height, 0 == height / 2
        amath::Vec4f pp = toFrameCoords(image, p);

        for (isize_t i = -1 * size; i < size; i++)
            for (isize_t j = -1 * size; j < size; j++)
                image.Set(pp.x + j, image.Height() - pp.y + i, c);
    };

    amath::Vec4f origin = amath::Point(0, 0, 0);
    for (usize_t deg = 0; deg < 360; deg += 30) {
        amath::Vec4f p = amath::RotateZ(amath::radians(deg)) * amath::Translate(0, 0.8, 0) * origin;
        drawPoint(p, 2, c);
    }
}

}  // namespace raytracer
