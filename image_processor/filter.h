#pragma once

#include "image.h"

class Filter {
public:
    virtual void ApplyFilter(Image &image) const = 0;
    virtual ~Filter() = default;
};