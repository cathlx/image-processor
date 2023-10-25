add_executable(
        image_processor
        image_processor.cpp
        image.cpp image.h filter.h parser.cpp parser.h crop.cpp crop.h grayscale.cpp grayscale.h negative.cpp negative.h sharpening.cpp sharpening.h edge.cpp edge.h blur.cpp blur.h crystal.cpp crystal.h color.cpp color.h convolution.cpp convolution.h)

add_catch(test_image_processor test.cpp image.cpp image.h filter.h parser.cpp parser.h crop.cpp crop.h grayscale.cpp grayscale.h negative.cpp negative.h sharpening.cpp sharpening.h edge.cpp edge.h blur.cpp blur.h crystal.cpp crystal.h color.cpp color.h convolution.cpp convolution.h)