#pragma once

#include "color.h"

class Grayscale : public ColorFilter {
public:
    ~Grayscale() override = default;

    void ApplyColorFilter(Image::Pixel &pixel) const override;
};