#include <catch.hpp>

#include "image.h"
#include "parser.h"
#include "crop.h"
#include "grayscale.h"
#include "negative.h"
#include "sharpening.h"
#include "edge.h"
#include "blur.h"
#include "crystal.h"

#include <set>

const std::string INPUT_PATH = "../tasks/image_processor/examples/shrek.bmp";
const double EPS = 0.01;
const double BIG_EPS = 15;

const int CROP_CONST = 150;
const double EDGE_CONST = 0.1;
const int CRYSTAL_CONST = 100;

bool EqualImages(Image& first, Image& second) {
    if (first.InfoHeader().width != second.Width() || first.InfoHeader().height != second.Height()) {
        return false;
    }
    for (size_t x = 0; x < second.Width(); ++x) {
        for (size_t y = 0; y < second.Height(); ++y) {
            for (size_t i = 0; i < 3; ++i) {
                if (std::abs(first.At(x, y, i) - second.At(x, y, i)) >= EPS) {
                    return false;
                }
            }
        }
    }
    return true;
}

size_t DifferentColors(Image& image) {
    std::set<Image::Pixel> colors;
    for (size_t x = 0; x < image.Width(); ++x) {
        for (size_t y = 0; y < image.Height(); ++y) {
            colors.insert(image[x][y]);
        }
    }
    return colors.size();
}

TEST_CASE("Crop") {
    Image image;
    image.Read(INPUT_PATH);

    Crop crop(CROP_CONST, CROP_CONST);
    crop.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_crop.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Grayscale") {
    Image image;
    image.Read(INPUT_PATH);

    Grayscale grayscale;
    grayscale.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_gs.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Negative") {
    Image image;
    image.Read(INPUT_PATH);

    Negative negative;
    negative.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_neg.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Sharpening") {
    Image image;
    image.Read(INPUT_PATH);

    Sharpening sharpening;
    sharpening.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_sharp.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Edge") {
    Image image;
    image.Read(INPUT_PATH);

    EdgeDetection edge(EDGE_CONST);
    edge.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_edge.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Blur") {
    Image image;
    image.Read(INPUT_PATH);

    GaussianBlur blur(3);
    blur.ApplyFilter(image);

    Image example;
    example.Read("../tasks/image_processor/examples/shrek_blur.bmp");

    REQUIRE(EqualImages(image, example));
}

TEST_CASE("Crystallize") {
    Image image;
    image.Read(INPUT_PATH);

    Crystallize crystal(CRYSTAL_CONST);
    crystal.ApplyFilter(image);

    REQUIRE(100 - DifferentColors(image) <= BIG_EPS);
}

TEST_CASE("Parser") {
    Parser parser;
    parser.HelpParse(9, {"./home/test.bmp", "./home/output/result.bmp", "-gs", "-blur", "6", "-edge", "0.9", "-sharp"});

    Parser::Commands actions = parser.Arguments();
    REQUIRE(actions.input_path == "./home/test.bmp");
    REQUIRE(actions.output_path == "./home/output/result.bmp");
    REQUIRE(actions.filters[0].name == "gs");
    REQUIRE(actions.filters[0].parameters.empty());
    REQUIRE(actions.filters[1].name == "blur");
    REQUIRE(actions.filters[1].parameters[0] == "6");
    REQUIRE(actions.filters[2].name == "edge");
    REQUIRE(actions.filters[2].parameters[0] == "0.9");
    REQUIRE(actions.filters[3].name == "sharp");
    REQUIRE(actions.filters[3].parameters.empty());
}

TEST_CASE("Reading file") {
    Image image;
    try {
        image.Read("/nonexistentpath/image.bmp");
    } catch (const std::invalid_argument& e) {
        REQUIRE(e.what() == std::string("Error: Invalid path. File could not be opened."));
    }
}

TEST_CASE("Writing file") {
    Image image;
    try {
        image.Read(INPUT_PATH);
        image.Save("/nonexistentpath/image.bmp");
    } catch (const std::invalid_argument& e) {
        REQUIRE(e.what() == std::string("Error: Invalid path. File could not be saved."));
    }
}

TEST_CASE("Crop with negative width") {
    Image image;
    try {
        image.Read(INPUT_PATH);
        Crop crop(-100, 100);
    } catch (const std::invalid_argument& e) {
        REQUIRE(e.what() == std::string("Error: Image width cannot be negative."));
    }
}

TEST_CASE("Crystallize too much crystals") {
    Parser parser;
    try {
        parser.HelpParse(5, {"inputpath", "outputpath", "-crystal", "100000000"});
    } catch (const std::invalid_argument& e) {
        REQUIRE(e.what() == std::string("Error: More crystals than pixels."));
    }
}

TEST_CASE("Crystallize zero crystals") {
    Parser parser;
    try {
        parser.HelpParse(5, {"inputpath", "outputpath", "-crystal", "0"});
    } catch (const std::invalid_argument& e) {
        REQUIRE(e.what() == std::string("Error: Must be more than zero crystals."));
    }
}
