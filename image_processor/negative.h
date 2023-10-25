#pragma once

#include "color.h"

class Negative : public ColorFilter {
public:
    ~Negative() override = default;

    void ApplyColorFilter(Image::Pixel &pixel) const override;
};