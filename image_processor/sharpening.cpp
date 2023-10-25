#include "sharpening.h"

void Sharpening::ApplyFilter(Image &image) const {
    ApplyConvolutionFilter(image);
}