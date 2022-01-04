#include "util.h"

namespace tga = anvil::file::tga;
namespace amath = anvil::math;

amath::Vec4f toFrameCoords(const tga::Image &image, amath::Vec4f p) {
    amath::Vec4f pp;
    pp.x = p.x < 0 ? image.Width() / 2 - (-1 * p.x) * image.Width() / 2 : image.Width() / 2 + p.x * image.Width() / 2;
    pp.y = p.y < 0 ? image.Height() / 2 - (-1 * p.y) * image.Height() / 2 : image.Height() / 2 + p.y * image.Height() / 2;

    return pp;
}
