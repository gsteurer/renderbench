#pragma once

#include "anvil/file/mesh.h"
#include "anvil/file/tga.h"

namespace renderer {

void drawWireframe(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh);
void drawMesh(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh);
void drawSampleLines(anvil::file::tga::Image &image);
void drawSampleTriangles(anvil::file::tga::Image &image);

}  // namespace renderer
