#pragma once

#include "filter.h"

class ConvolutionFilter : public Filter {
public:
    explicit ConvolutionFilter(const std::vector<std::vector<double>> &kernel) : kernel_(kernel) {
    }
    ~ConvolutionFilter() override = default;

    void ApplyConvolutionFilter(Image &image) const;

protected:
    std::vector<std::vector<double>> kernel_;
};