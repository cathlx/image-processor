#include "crop.h"

#include <stdexcept>

void Crop::ApplyFilter(Image &image) const {
    if (width_ < 0) {
        throw std::invalid_argument("Error: Image width cannot be negative.");
    }
    if (height_ < 0) {
        height_ = std::abs(height_);
        inverted_ = true;
    }
    if (width_ > image.Width()) {
        width_ = static_cast<int>(image.Width());
    }
    if (height_ > image.Height()) {
        height_ = static_cast<int>(image.Height());
    }
    image.Resize(width_, height_);
    if (inverted_) {
        Invert(image);
    }
    image.InfoHeader().width = width_;
    image.InfoHeader().height = height_;
}

void Crop::Invert(Image &image) const {
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y <= image.Height() / 2; ++y) {
            for (size_t i = 0; i < 3; ++i) {
                double tmp = image[x][y][i];
                image[x][y][i] = image[x][image.Height() - y - 1][i];
                image[x][image.Height() - y - 1][i] = tmp;
            }
        }
    }
}