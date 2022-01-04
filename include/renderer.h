#pragma once

#include "anvil/file/mesh.h"
#include "anvil/file/tga.h"

namespace renderer {

void drawLines(anvil::file::tga::Image &image);
void drawWireframe(anvil::file::tga::Image &image, const anvil::file::Mesh &mesh);

}  // namespace renderer
