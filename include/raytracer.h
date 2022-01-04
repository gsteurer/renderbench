#pragma once

#include "anvil/file/tga.h"

namespace raytracer {

void drawLine(anvil::file::tga::Image &image);

// raytracer challenge chapter 3
void drawProjectile(anvil::file::tga::Image &image);

// raytracer challenge chapter 4
void drawClockFace(anvil::file::tga::Image &image);

}  // namespace raytracer
