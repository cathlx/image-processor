#pragma once

#include "filter.h"

class Crystallize : public Filter {
public:
    explicit Crystallize(int crystals) : crystals_(crystals){};
    ~Crystallize() override = default;

    void ApplyFilter(Image &image) const override;

private:
    int crystals_;
};