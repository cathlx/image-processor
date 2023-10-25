#pragma once

#include "filter.h"

class GaussianBlur : public Filter {
public:
    explicit GaussianBlur(const double &sigma);
    ~GaussianBlur() override = default;

    void ApplyFilter(Image &image) const override;

private:
    std::vector<double> kernel_;
    size_t kernel_size_;
    size_t kernel_radius_;
};