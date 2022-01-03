
#include <stddef.h>  // size_t
#include <stdint.h>  // intn_t
#include <string.h>  // memset

#include <fstream>
#include <iostream>
#include <string>

#include "anvil/file/tga.h"
#include "anvil/math/mat4x4f.h"
#include "anvil/math/util.h"
#include "anvil/math/vec4f.h"

namespace tga = anvil::file::tga;
namespace amath = anvil::math;

void drawLine() {
    const size_t width = 600;
    const size_t height = 800;
    tga::Image image(width, height, sizeof(tga::Color));
    image.Clear(tga::Color(0, 0, 0, 1.0));
    for (size_t idx = 0; idx < width; idx++) {
        image.Set(idx, (height / 2) - 1, tga::Color(1.0, 0, 0, 1.0));
        image.Set(idx, height / 2, tga::Color(0, 1.0, 0, 1.0));
        image.Set(idx, (height / 2) + 1, tga::Color(0, 0, 1.0, 1.0));
    }
    if (auto status = image.WriteTGA("frame.tga"); !status)
        std::cerr << "failed to write image!" << std::endl;
}

void drawProjectile() {
    const size_t width = 900;
    const size_t height = 500;
    tga::Image image(width, height, sizeof(tga::Color));
    image.Clear(tga::Color(0, 0, 0, 1.0));

    amath::Vec4f p(0.0, 1.0, 0.0, 1.0);
    amath::Vec4f v(1.0, 1.8, 0.0, 0.0);
    v = amath::norm(v) * 11.25;
    amath::Vec4f g(0.0, -0.1, 0.0, 0.0);
    amath::Vec4f w(-0.01, 0.0, 0.0, 0.0);

    for (size_t idx = 0; idx < width; idx++) {
        p = p + v;
        v = v + g + w;
        image.Set(p.x, height - p.y, tga::Color(1.0, 0.0, 0.0, 1.0));
    }

    if (auto status = image.WriteTGA("frame.tga"); !status)
        std::cerr << "failed to write image!" << std::endl;
}

// raytracer challenge chapter 4
void drawClockFace() {
    const size_t width = 400;
    const size_t height = 400;

    tga::Image image(width, height, sizeof(tga::Color));
    image.Clear(tga::Color(0, 0, 0, 1.0));
    tga::Color c = tga::Color(1.0, 1.0, 1.0, 1.0);

    auto drawPoint = [&image, c](amath::Vec4f p, isize_t size) {
        //x: -1 == 0, 1 == width, 0 == width / 2
        //y: -1 == 0, 1 == height, 0 == height / 2
        amath::Vec4f pp;
        pp.x = p.x < 0 ? image.Width() / 2 - (-1 * p.x) * image.Width() / 2 : image.Width() / 2 + p.x * image.Width() / 2;
        pp.y = p.y < 0 ? image.Height() / 2 - (-1 * p.y) * image.Height() / 2 : image.Height() / 2 + p.y * image.Height() / 2;

        for (isize_t i = -1 * size; i < size; i++)
            for (isize_t j = -1 * size; j < size; j++)
                image.Set(pp.x + j, image.Height() - pp.y + i, c);
    };

    amath::Vec4f origin = amath::Point(0, 0, 0);
    for (usize_t deg = 0; deg < 360; deg += 30) {
        amath::Vec4f p = amath::RotateZ(amath::radians(deg)) * amath::Translate(0, 0.8, 0) * origin;
        drawPoint(p, 2);
    }

    if (auto status = image.WriteTGA("frame.tga"); !status)
        std::cerr << "failed to write image!" << std::endl;
}

int main() {
    drawClockFace();
    return 0;
}