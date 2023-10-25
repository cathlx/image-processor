#pragma once

#include "image.h"

const int FIVE = 5;

class Filter {
public:
    virtual void ApplyFilter(Image &image) const = 0;

    virtual ~Filter() = default;
};

class ConvolutionFilter : public Filter {
public:
    ~ConvolutionFilter() override = default;

    explicit ConvolutionFilter(const std::vector<std::vector<double>> &kernel) : kernel_(kernel) {
    }

    void ApplyConvolutionFilter(Image &image) const;

protected:
    std::vector<std::vector<double>> kernel_;
};

class ColorFilter : public Filter {
public:
    ~ColorFilter() override = default;

    void ApplyFilter(Image &image) const override;

protected:
    virtual void ApplyColorFilter(Image::Pixel &pixel) const = 0;
};

class Crop : public Filter {
public:
    Crop(const int &width, const int &height) : width_(width), height_(height){};

    ~Crop() override = default;

    void ApplyFilter(Image &image) const override;

private:
    mutable int width_;
    mutable int height_;
};

class Grayscale : public ColorFilter {
public:
    ~Grayscale() override = default;

    void ApplyColorFilter(Image::Pixel &pixel) const override;
};

class Negative : public ColorFilter {
public:
    ~Negative() override = default;

    void ApplyColorFilter(Image::Pixel &pixel) const override;
};

class Sharpening : public ConvolutionFilter {
public:
    Sharpening() : ConvolutionFilter({{0, -1, 0}, {-1, FIVE, -1}, {0, -1, 0}}){};

    ~Sharpening() override = default;

    void ApplyFilter(Image &image) const override;
};

class EdgeDetection : public ConvolutionFilter {
public:
    explicit EdgeDetection(double threshold)
        : ConvolutionFilter({{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}), threshhold_(threshold){};

    ~EdgeDetection() override = default;

    void ApplyFilter(Image &image) const override;

private:
    double threshhold_;
};

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(const double &sigma);

    ~GaussianBlur() override = default;

    void ApplyFilter(Image &image) const override;

private:
    std::vector<double> kernel_;
};

class Crystallize : public Filter {
public:
    explicit Crystallize(int crystals) : crystals_(crystals){};

    ~Crystallize() override = default;

    void ApplyFilter(Image &image) const override;

private:
    int crystals_;
};