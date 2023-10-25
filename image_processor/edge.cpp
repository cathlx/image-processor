#include "edge.h"
#include "grayscale.h"

void EdgeDetection::ApplyFilter(Image &image) const {
    Grayscale grayscale;
    grayscale.ApplyFilter(image);
    ApplyConvolutionFilter(image);
    for (int x = 0; x < image.Width(); ++x) {
        for (int y = 0; y < image.Height(); ++y) {
            if (image[x][y][0] > threshhold_ || image[x][y][1] > threshhold_ || image[x][y][2] > threshhold_) {
                for (size_t i = 0; i < 3; ++i) {
                    image[x][y][i] = 1.0;
                }
            } else {
                for (size_t i = 0; i < 3; ++i) {
                    image[x][y][i] = 0.0;
                }
            }
        }
    }
}