
#include <iostream>

#include "anvil/file/tga.h"
#include "anvil/types.h"
#include "raytracer.h"
#include "renderer.h"

namespace tga = anvil::file::tga;

int main() {
    const isize_t width = 800;
    const isize_t height = 600;

    tga::Image image(width, height, sizeof(tga::Color));
    image.Clear(tga::Color(0, 0, 0, 1.0));

    renderer::drawLines(image);

    if (auto status = image.WriteTGA("frame.tga"); !status)
        std::cerr << "failed to write image!" << std::endl;

    return 0;
}