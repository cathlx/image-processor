#include "convolution.h"

void ConvolutionFilter::ApplyConvolutionFilter(Image &image) const {
    int max_width = std::max(0, static_cast<int>(image.Width()) - 1);
    int max_height = std::max(0, static_cast<int>(image.Height()) - 1);
    for (size_t i = 0; i < 3; ++i) {
        std::vector<double> previous_column;
        std::vector<double> current_column;
        for (int x = 0; x < image.Width(); ++x) {
            for (int y = 0; y < image.Height(); ++y) {
                int lwb = std::max(0, x - 1);
                int lhb = std::max(0, y - 1);
                int uwb = std::min(max_width, x + 1);
                int uhb = std::min(max_height, y + 1);
                double d = std::min(
                    1.0, std::max(0.0, kernel_[0][0] * image[lwb][lhb][i] + kernel_[0][1] * image[x][lhb][i] +
                                           kernel_[0][2] * image[uwb][lhb][i] + kernel_[1][0] * image[lwb][y][i] +
                                           kernel_[1][1] * image[x][y][i] + kernel_[1][2] * image[uwb][y][i] +
                                           kernel_[2][0] * image[lwb][uhb][i] + kernel_[2][1] * image[x][uhb][i] +
                                           kernel_[2][2] * image[uwb][uhb][i]));
                current_column.push_back(d);
            }
            if (x > 0) {
                for (int z = 0; z < image.Height(); ++z) {
                    image[x - 1][z][i] = previous_column[z];
                }
            }
            if (x == image.Width() - 1) {
                for (int z = 0; z < image.Height(); ++z) {
                    image[x][z][i] = current_column[z];
                }
            }
            previous_column = current_column;
            current_column.clear();
        }
    }
}