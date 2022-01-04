#pragma once

#include "anvil/file/tga.h"
#include "anvil/math/vec4f.h"

anvil::math::Vec4f toFrameCoords(
    const anvil::file::tga::Image &image,
    anvil::math::Vec4f p);