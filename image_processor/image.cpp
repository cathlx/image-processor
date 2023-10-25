#include "image.h"

#include <iostream>
#include <stdexcept>

const double N = 255.0;
const uint16_t BM = 19778;
const uint16_t BPP = 24;

Image::Image(const size_t& width, const size_t& height) {
    image_.resize(width, std::vector<Image::Pixel>(height));
    for (size_t x = 0; x < width; ++x) {
        for (size_t y = 0; y < height; ++y) {
            image_[x][y] = {0, 0, 0};
        }
    }
}

size_t Image::Width() const {
    return image_.size();
}

size_t Image::Height() const {
    return image_[0].size();
}

double& Image::At(const size_t& x, const size_t& y, const size_t& i) {
    return image_[x][y][i];
}

void Image::Resize(const int& width, const int& height) {
    image_.resize(width, std::vector<Image::Pixel>(height));
}

std::vector<Image::Pixel>& Image::operator[](size_t i) {
    return image_[i];
}

void Image::Read(const std::string& path) {
    std::ifstream f(path, std::ios::binary);
    if (!f.is_open()) {
        throw std::invalid_argument("Error: Invalid path. File could not be opened.");
    }

    f.read(reinterpret_cast<char*>(&file_header_), sizeof(BMPFileHeader));

    if (file_header_.format != BM) {
        throw std::invalid_argument("Error: Wrong file format.");
    }

    f.read(reinterpret_cast<char*>(&info_header_), sizeof(BMPInfoHeader));

    if (info_header_.bpp != BPP) {
        throw std::invalid_argument("Error: Wrong file format.");
    }

    int width = static_cast<int>(info_header_.width);
    int height = static_cast<int>(info_header_.height);
    const int padding = ((4 - (width * 3) % 4) % 4);

    image_.resize(width, std::vector<Image::Pixel>(height));

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char color[3];
            f.read(reinterpret_cast<char*>(color), 3);
            image_[x][height - y - 1].resize(3);
            image_[x][height - y - 1][0] = static_cast<double>(color[2]) / N;
            image_[x][height - y - 1][1] = static_cast<double>(color[1]) / N;
            image_[x][height - y - 1][2] = static_cast<double>(color[0]) / N;
        }
        f.ignore(padding);
    }
    f.close();
}

void Image::Save(const std::string& path) {
    std::ofstream f;
    f.open(path, std::ios::out | std::ios::binary);

    if (!f.is_open()) {
        throw std::invalid_argument("Error: Invalid path. File could not be saved.");
    }

    f.write(reinterpret_cast<char*>(&file_header_), sizeof(BMPFileHeader));
    f.write(reinterpret_cast<char*>(&info_header_), sizeof(BMPInfoHeader));

    int width = static_cast<int>(info_header_.width);
    int height = static_cast<int>(info_header_.height);
    const int padding = ((4 - (width * 3) % 4) % 4);

    unsigned char pad[3] = {0, 0, 0};
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            unsigned char r = static_cast<unsigned char>(image_[x][height - y - 1][0] * N);
            unsigned char g = static_cast<unsigned char>(image_[x][height - y - 1][1] * N);
            unsigned char b = static_cast<unsigned char>(image_[x][height - y - 1][2] * N);

            unsigned char color[] = {b, g, r};
            f.write(reinterpret_cast<char*>(color), 3);
        }
        f.write(reinterpret_cast<char*>(pad), padding);
    }
    f.close();
}

Image::BMPInfoHeader& Image::InfoHeader() {
    return info_header_;
}