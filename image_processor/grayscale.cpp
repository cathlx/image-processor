#include "grayscale.h"

void Grayscale::ApplyColorFilter(Image::Pixel &pixel) const {
    const double red = 0.299;
    const double green = 0.587;
    const double blue = 0.114;

    double t = pixel[0] * red + pixel[1] * green + pixel[2] * blue;
    for (auto &color : pixel) {
        color = std::min(t, 1.0);
    }
}