#pragma once

#include "convolution.h"

const int FIVE = 5;

class Sharpening : public ConvolutionFilter {
public:
    Sharpening() : ConvolutionFilter({{0, -1, 0}, {-1, FIVE, -1}, {0, -1, 0}}){};
    ~Sharpening() override = default;

    void ApplyFilter(Image &image) const override;
};