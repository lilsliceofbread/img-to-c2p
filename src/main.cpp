#include <iostream>
#include <memory>
#include "C2PImage.hpp"

//SURVEY FOR MEMORY LEAKS -> USING =nullptr INSTEAD OF free()

int main() {
    std::string filename; //no setting of max size causes segfaults
    std::cout << "Img to C2P program" << std::endl;

    std::cout << "Input image path: " << std::endl;
    std::getline(std::cin, filename);

    std::unique_ptr<C2PImage> image = std::make_unique<C2PImage>(filename.c_str(), ImageFormat::PNG);
    if(image->ConvertToC2P()) { //if conversion doesn't fail write
        if (!image->Write("outputimg")) //if write does fail err
            std::cerr << "ERR: image write failed" << std::endl;
    }
    return 0;
}