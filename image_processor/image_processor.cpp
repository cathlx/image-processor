#include "image.h"
#include "parser.h"
#include "crop.h"
#include "grayscale.h"
#include "negative.h"
#include "sharpening.h"
#include "edge.h"
#include "blur.h"
#include "crystal.h"

#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
    try {
        Parser parser(argc, argv);
        Parser::Commands actions = parser.Arguments();
        Image image;
        image.Read(actions.input_path);
        for (auto& filter : actions.filters) {
            if (filter.name == "crop") {
                int width = static_cast<int>(std::stod(filter.parameters[0]));
                int height = static_cast<int>(std::stod(filter.parameters[1]));
                Crop crop(width, height);
                crop.ApplyFilter(image);
            } else if (filter.name == "gs") {
                Grayscale grayscale;
                grayscale.ApplyFilter(image);
            } else if (filter.name == "neg") {
                Negative negative;
                negative.ApplyFilter(image);
            } else if (filter.name == "sharp") {
                Sharpening sharpening;
                sharpening.ApplyFilter(image);
            } else if (filter.name == "edge") {
                EdgeDetection edge(std::stod(filter.parameters[0]));
                edge.ApplyFilter(image);
            } else if (filter.name == "blur") {
                GaussianBlur blur(std::stod(filter.parameters[0]));
                blur.ApplyFilter(image);
            } else if (filter.name == "crystal") {
                int crystals = static_cast<int>(std::stod(filter.parameters[0]));
                if (crystals > image.Height() * image.Width()) {
                    throw std::invalid_argument("Error: More crystals than pixels.");
                } else if (crystals == 0) {
                    throw std::invalid_argument("Error: Must be more than zero crystals.");
                }
                Crystallize crystal(crystals);
                crystal.ApplyFilter(image);
            } else {
                throw std::invalid_argument("Error: Invalid program arguments.");
            }
        }
        image.Save(actions.output_path);
    } catch (const std::invalid_argument& e) {
        std::cerr << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Program error. Try again." << std::endl;
    }
    return 0;
}