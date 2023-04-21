#include <iostream>
#include "C2PImage.hpp"

int main() {
    std::string filename; //no setting of max size causes segfaults
    std::cout << "Img to C2P program" << std::endl;

    std::cout << "Input image path: " << std::endl;
    std::getline(std::cin, filename);

    C2PImage image(filename.c_str(), ImageFormat::PNG, 3);
    if(image.ConvertToC2P()) { //if conversion doesn't fail write
        if (!image.Write("outputimg")) //if write does fail err
            std::cerr << "ERR: image write failed" << std::endl;
    }
    return 0;
}