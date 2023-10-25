#pragma once

#include "convolution.h"

class EdgeDetection : public ConvolutionFilter {
public:
    explicit EdgeDetection(double threshold)
        : ConvolutionFilter({{0, -1, 0}, {-1, 4, -1}, {0, -1, 0}}), threshhold_(threshold){};
    ~EdgeDetection() override = default;

    void ApplyFilter(Image &image) const override;

private:
    double threshhold_;
};