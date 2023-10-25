#include "crystal.h"

#include <limits>
#include <random>
#include <utility>

void Crystallize::ApplyFilter(Image &image) const {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> width(0, static_cast<int>(image.Width()) - 1);
    std::uniform_int_distribution<> height(0, static_cast<int>(image.Height()) - 1);

    std::vector<std::pair<int, int>> crystals;
    for (size_t i = 0; i < crystals_; ++i) {
        crystals.push_back(std::make_pair(width(gen), height(gen)));
    }

    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            double distance = std::numeric_limits<double>::max();
            size_t closest_index = 0;
            for (size_t i = 0; i < crystals_; ++i) {
                double d = std::sqrt(((x - crystals[i].first) * (x - crystals[i].first)) +
                                     (y - crystals[i].second) * (y - crystals[i].second));
                if (distance > d) {
                    distance = d;
                    closest_index = i;
                }
            }
            image[x][y] = image[crystals[closest_index].first][crystals[closest_index].second];
        }
    }
}