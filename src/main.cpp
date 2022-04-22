
#include <iostream>

#include "anvil/file/mesh.h"
#include "anvil/file/tga.h"
#include "anvil/file/wavefront.h"
#include "anvil/types.h"
#include "raytracer.h"
#include "renderer.h"

namespace tga = anvil::file::tga;
namespace afile = anvil::file;
namespace wf = anvil::file::wavefront;

int main() {
    afile::Mesh mesh = wf::Parse("assets/suzanne.obj");

    const isize_t width = 1000;
    const isize_t height = 1000;

    tga::Image image(width, height, sizeof(tga::Color));
    image.Clear(tga::Color(0, 0, 0, 1.0));

    // renderer::drawWireframe(image, mesh);
    renderer::drawMesh(image, mesh);
    // renderer::drawSampleTriangles(image);

    if (auto status = image.WriteTGA("frame.tga"); !status)
        std::cerr << "failed to write image!" << std::endl;

    return 0;
}