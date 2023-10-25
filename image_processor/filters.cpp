#include "filters.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <numbers>
#include <random>
#include <utility>

const int SIX = 6;

void ConvolutionFilter::ApplyConvolutionFilter(Image &image) const {
    int max_width = std::max(0, static_cast<int>(image.Width()) - 1);
    int max_height = std::max(0, static_cast<int>(image.Height()) - 1);
    for (size_t i = 0; i < 3; ++i) {
        std::vector<double> previous_column;
        std::vector<double> current_column;
        for (int x = 0; x < image.Width(); ++x) {
            for (int y = 0; y < image.Height(); ++y) {
                int lwb = std::max(0, x - 1);
                int lhb = std::max(0, y - 1);
                int uwb = std::min(max_width, x + 1);
                int uhb = std::min(max_height, y + 1);
                double d = std::min(
                    1.0, std::max(0.0, kernel_[0][0] * image[lwb][lhb][i] + kernel_[0][1] * image[x][lhb][i] +
                                           kernel_[0][2] * image[uwb][lhb][i] + kernel_[1][0] * image[lwb][y][i] +
                                           kernel_[1][1] * image[x][y][i] + kernel_[1][2] * image[uwb][y][i] +
                                           kernel_[2][0] * image[lwb][uhb][i] + kernel_[2][1] * image[x][uhb][i] +
                                           kernel_[2][2] * image[uwb][uhb][i]));
                current_column.push_back(d);
            }
            if (x > 0) {
                for (int z = 0; z < image.Height(); ++z) {
                    image[x - 1][z][i] = previous_column[z];
                }
            }
            if (x == image.Width() - 1) {
                for (int z = 0; z < image.Height(); ++z) {
                    image[x][z][i] = current_column[z];
                }
            }
            previous_column = current_column;
            current_column.clear();
        }
    }
}

void ColorFilter::ApplyFilter(Image &image) const {
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            ApplyColorFilter(image[x][y]);
        }
    }
}

void Crop::ApplyFilter(Image &image) const {
    if (width_ < 0) {
        std::cout << "Error: Image width cannot be negative." << std::endl;
        exit(0);
    }
    if (height_ < 0) {
        std::cout << "Error: Image height cannot be negative." << std::endl;
        exit(0);
    }
    if (width_ > image.Width()) {
        width_ = static_cast<int>(image.Width());
    }
    if (height_ > image.Height()) {
        height_ = static_cast<int>(image.Height());
    }
    image.Resize(width_, height_);
    image.InfoHeader().width = width_;
    image.InfoHeader().height = height_;
}

void Grayscale::ApplyColorFilter(Image::Pixel &pixel) const {
    const double red = 0.299;
    const double green = 0.587;
    const double blue = 0.114;

    double t = pixel[0] * red + pixel[1] * green + pixel[2] * blue;
    for (auto &color : pixel) {
        color = std::min(t, 1.0);
    }
}

void Negative::ApplyColorFilter(Image::Pixel &pixel) const {
    for (auto &color : pixel) {
        color = 1 - color;
    }
}

void Sharpening::ApplyFilter(Image &image) const {
    ApplyConvolutionFilter(image);
}

void EdgeDetection::ApplyFilter(Image &image) const {
    Grayscale grayscale;
    grayscale.ApplyFilter(image);
    ApplyConvolutionFilter(image);
    for (int x = 0; x < image.Width(); ++x) {
        for (int y = 0; y < image.Height(); ++y) {
            if (image[x][y][0] > threshhold_ || image[x][y][1] > threshhold_ || image[x][y][2] > threshhold_) {
                for (size_t i = 0; i < 3; ++i) {
                    image[x][y][i] = 1.0;
                }
            } else {
                for (size_t i = 0; i < 3; ++i) {
                    image[x][y][i] = 0.0;
                }
            }
        }
    }
}

GaussianBlur::GaussianBlur(const double &sigma) {
    int kernel_size = static_cast<int>(std::ceil(SIX * std::abs(sigma)));
    if (kernel_size % 2 == 0) {
        ++kernel_size;
    }
    int half_kernel_size = kernel_size / 2;
    kernel_.resize(kernel_size);

    for (int i = 0; i <= half_kernel_size; ++i) {
        kernel_[half_kernel_size - i] = std::exp(-static_cast<double>(i * i) / (2 * sigma * sigma)) /
                                        std::sqrt(2 * std::numbers::pi * sigma * sigma);
        kernel_[half_kernel_size + i] = kernel_[half_kernel_size - i];
    }
}

void GaussianBlur::ApplyFilter(Image &image) const {
    int half_kernel_size = static_cast<int>(kernel_.size() / 2);

    Image help(image.Width(), image.Height());

    for (int x = 0; x < image.Width(); ++x) {
        for (int y = 0; y < image.Height(); ++y) {
            for (size_t i = 0; i < 3; ++i) {
                for (int j = -half_kernel_size; j <= half_kernel_size; ++j) {
                    int h = y + j;
                    if (h < 0) {
                        h = 0;
                    } else if (h >= image.Height()) {
                        h = static_cast<int>(image.Height() - 1);
                    }
                    help[x][y][i] += image[x][h][i] * kernel_[j + half_kernel_size];
                }
                help[x][y][i] = std::min(help[x][y][i], 1.0);
            }
        }
    }

    for (int y = 0; y < image.Height(); ++y) {
        for (int x = 0; x < image.Width(); ++x) {
            for (size_t i = 0; i < 3; ++i) {
                image[x][y][i] = 0;
                for (int j = -half_kernel_size; j <= half_kernel_size; ++j) {
                    int w = x + j;
                    if (w < 0) {
                        w = 0;
                    } else if (w >= image.Width()) {
                        w = static_cast<int>(image.Height() - 1);
                    }
                    image[x][y][i] += help[w][y][i] * kernel_[j + half_kernel_size];
                }
                image[x][y][i] = std::min(image[x][y][i], 1.0);
            }
        }
    }
}

void Crystallize::ApplyFilter(Image &image) const {
    Image result(image.Width(), image.Height());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width(0, static_cast<int>(image.Width()) - 1);
    std::uniform_int_distribution<> height(0, static_cast<int>(image.Height()) - 1);

    std::vector<std::pair<int, int>> crystals;
    for (size_t i = 0; i < crystals_; ++i) {
        crystals.push_back(std::make_pair(width(gen), height(gen)));
    }

    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            double distance = std::numeric_limits<double>::max();
            size_t closest_index = 0;
            for (size_t i = 0; i < crystals_; ++i) {
                double d = std::sqrt(((x - crystals[i].first) * (x - crystals[i].first)) +
                                     (y - crystals[i].second) * (y - crystals[i].second));
                if (distance > d) {
                    distance = d;
                    closest_index = i;
                }
            }
            image[x][y] = image[crystals[closest_index].first][crystals[closest_index].second];
        }
    }
}