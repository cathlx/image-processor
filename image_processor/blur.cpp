#include "blur.h"

#include <cmath>
#include <numbers>

const int SIX = 6;

GaussianBlur::GaussianBlur(const double &sigma) {
    kernel_size_ = static_cast<size_t>(std::ceil(SIX * sigma));
    kernel_radius_ = kernel_size_ / 2;
    kernel_.resize(kernel_radius_ + 1);

    for (size_t i = 0; i <= kernel_radius_; ++i) {
        kernel_[i] = std::exp(-static_cast<double>(i * i) / (2 * sigma * sigma)) /
                     std::sqrt(2 * std::numbers::pi * sigma * sigma);
    }
}

void GaussianBlur::ApplyFilter(Image &image) const {
    std::vector<std::vector<Image::Pixel>> tmp;
    tmp.resize(image.Width(), std::vector<Image::Pixel>(image.Height()));

    for (size_t i = 0; i < 3; ++i) {
        for (size_t x = 0; x < image.Width(); ++x) {
            for (size_t y = 0; y < kernel_radius_; ++y) {
                tmp[x][y].push_back(image[x][y][i] * kernel_[0]);
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    tmp[x][y][i] += (image[x][0][i] + image[x][y + j][i]) * kernel_[j];
                }
            }
            for (size_t y = kernel_radius_; y < image.Height() - kernel_radius_; ++y) {
                tmp[x][y].push_back(image[x][y][i] * kernel_[0]);
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    tmp[x][y][i] += (image[x][y - j][i] + image[x][y + j][i]) * kernel_[j];
                }
            }
            for (size_t y = image.Height() - kernel_radius_; y < image.Height(); ++y) {
                tmp[x][y].push_back(image[x][y][i] * kernel_[0]);
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    tmp[x][y][i] += (image[x][y - j][i] + image[x][image.Height() - 1][i]) * kernel_[j];
                }
            }
        }

        for (size_t y = 0; y < image.Height(); ++y) {
            for (size_t x = 0; x < kernel_radius_; ++x) {
                image[x][y][i] = tmp[x][y][i] * kernel_[0];
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    image[x][y][i] += (tmp[0][y][i] + tmp[x + j][y][i]) * kernel_[j];
                }
            }
            for (size_t x = kernel_radius_; x < image.Width() - kernel_radius_; ++x) {
                image[x][y][i] = tmp[x][y][i] * kernel_[0];
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    image[x][y][i] += (tmp[x - j][y][i] + tmp[x + j][y][i]) * kernel_[j];
                }
            }
            for (size_t x = image.Width() - kernel_radius_; x < image.Width(); ++x) {
                image[x][y][i] = tmp[x][y][i] * kernel_[0];
                for (size_t j = 1; j <= kernel_radius_; ++j) {
                    image[x][y][i] += (tmp[x - j][y][i] + tmp[image.Width() - 1][y][i]) * kernel_[j];
                }
            }
        }
    }
}