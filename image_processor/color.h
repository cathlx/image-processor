#pragma once

#include "filter.h"

class ColorFilter : public Filter {
public:
    ~ColorFilter() override = default;

    void ApplyFilter(Image &image) const override;

protected:
    virtual void ApplyColorFilter(Image::Pixel &pixel) const = 0;
};