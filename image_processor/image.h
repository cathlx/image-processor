#pragma once

#include <fstream>
#include <string>
#include <vector>

class Image {
public:
    using Pixel = std::vector<double>;

#pragma pack(push, 1)
    struct BMPFileHeader {
        uint16_t format;
        uint32_t file_size;
        uint16_t reserved1;
        uint16_t reserved2;
        uint32_t offset_pixels;
    };

    struct BMPInfoHeader {
        uint32_t header_size;
        int32_t width;
        int32_t height;
        uint16_t planes;
        uint16_t bpp;
        uint32_t compression;
        uint32_t raw_size;
        int32_t horizontal_resolution;
        int32_t vertical_resolution;
        uint32_t colors;
        uint32_t important_colors;
    };
#pragma pack(pop)

    Image() = default;
    Image(const size_t& width, const size_t& height);
    ~Image() = default;

    size_t Width() const;

    size_t Height() const;

    double& At(const size_t& x, const size_t& y, const size_t& i);

    BMPInfoHeader& InfoHeader();

    void Resize(const int& width, const int& height);

    std::vector<Pixel>& operator[](size_t i);

    void Read(const std::string& path);

    void Save(const std::string& path);

private:
    std::vector<std::vector<Pixel>> image_;
    BMPFileHeader file_header_;
    BMPInfoHeader info_header_;
};