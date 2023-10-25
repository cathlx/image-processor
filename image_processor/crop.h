#include "filter.h"

class Crop : public Filter {
public:
    Crop(const int &width, const int &height) : width_(width), height_(height){};
    ~Crop() override = default;

    void ApplyFilter(Image &image) const override;

    void Invert(Image &image) const;

private:
    mutable int width_;
    mutable int height_;
    mutable bool inverted_ = false;
};