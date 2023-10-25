#include "color.h"

void ColorFilter::ApplyFilter(Image &image) const {
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            ApplyColorFilter(image[x][y]);
        }
    }
}