#include "negative.h"

void Negative::ApplyColorFilter(Image::Pixel &pixel) const {
    for (auto &color : pixel) {
        color = 1 - color;
    }
}