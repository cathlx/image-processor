#include "parser.h"

#include <iostream>
#include <stdexcept>

bool IsInteger(std::string &string) {
    return string.find_first_not_of("0123456789-") == std::string::npos;
}

bool IsDouble(std::string &string) {
    return string.find_first_not_of("0123456789.") == std::string::npos;
}

Parser::Parser(int argc, char **argv) {
    Parse(argc, argv);
}

void Parser::Parse(int argc, char **argv) {
    if (argc == 1) {
        std::cout << "The program applies simple filters to an image.\n" << std::endl;
        std::cout << "It takes the path to the image to be processed, the path to upload the processed" << std::endl;
        std::cout << "image to, and a list of filters to be applied as arguments." << std::endl;
        std::cout << "The list of available filters can be found below:\n" << std::endl;
        std::cout << " -crop width height  Crops the image as specified (using upper left corner)." << std::endl;
        std::cout << " -gs                 Makes the image grey." << std::endl;
        std::cout << " -neg                Makes a negative." << std::endl;
        std::cout << " -sharp              Sharpens the image." << std::endl;
        std::cout << " -edge threshold     Performs edge detection." << std::endl;
        std::cout << " -blur sigma         Applies Gaussian Blur." << std::endl;
        std::cout << " -crystal crystals   Crystallizes the image." << std::endl;
        exit(0);
    }
    if (argc < 3) {
        throw std::invalid_argument("Error: no input path.");
    }
    if (argc < 4) {
        throw std::invalid_argument("Error: no output path.");
    }

    std::vector<std::string> args(argv + 1, argv + argc);
    HelpParse(argc, args);
}

void Parser::HelpParse(int argc, std::vector<std::string> args) {
    commands_.input_path = args[0];
    commands_.output_path = args[1];
    for (size_t i = 2; i < argc - 1; ++i) {
        std::string arg = args[i];
        if (arg == "-crop") {
            if (IsInteger(args[i + 1]) && IsInteger(args[i + 2])) {
                Filter filter;
                filter.name = "crop";
                filter.parameters.push_back(args[i + 1]);
                filter.parameters.push_back(args[i + 2]);
                commands_.filters.push_back(filter);
                i += 2;
            } else {
                throw std::invalid_argument("Error: Invalid crop arguments.");
            }
        } else if (arg == "-gs") {
            Filter filter;
            filter.name = "gs";
            commands_.filters.push_back(filter);
        } else if (arg == "-neg") {
            Filter filter;
            filter.name = "neg";
            commands_.filters.push_back(filter);
        } else if (arg == "-sharp") {
            Filter filter;
            filter.name = "sharp";
            commands_.filters.push_back(filter);
        } else if (arg == "-edge") {
            if (IsDouble(args[i + 1])) {
                Filter filter;
                filter.name = "edge";
                filter.parameters.push_back(args[i + 1]);
                commands_.filters.push_back(filter);
                i += 1;
            } else {
                throw std::invalid_argument("Error: Invalid edge argument.");
            }
        } else if (arg == "-blur") {
            if (IsDouble(args[i + 1])) {
                Filter filter;
                filter.name = "blur";
                filter.parameters.push_back(args[i + 1]);
                commands_.filters.push_back(filter);
                i += 1;
            } else {
                throw std::invalid_argument("Error: Invalid blur argument.");
            }
        } else if (arg == "-crystal") {
            if (IsInteger(args[i + 1])) {
                Filter filter;
                filter.name = "crystal";
                filter.parameters.push_back(args[i + 1]);
                commands_.filters.push_back(filter);
                i += 1;
            } else {
                throw std::invalid_argument("Error: Invalid crystallize argument.");
            }
        } else {
            throw std::invalid_argument("Error: Invalid input.");
        }
    }
}

Parser::Commands &Parser::Arguments() {
    return commands_;
}
