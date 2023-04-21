#include "C2PImage.hpp"
#include <iostream>
#include <fstream>

#define MAX_WIDTH 310
#define MAX_HEIGHT 401
#define COLOUR_CHANNELS 3

C2PImage::C2PImage(const char* image_path, ImageFormat output_format, int colour_channels) 
: Image(image_path, output_format, colour_channels) {
    std::cout << "c2pimage class constructed" << std::endl;
}

bool C2PImage::ConvertToC2P() {
    if (!Resize(MAX_WIDTH, MAX_HEIGHT)) {
        std::cerr << "ERR: image resize failed" << std::endl;
        return false;
    }
    if (!ConvertRGB565()) {
        std::cerr << "ERR: image RGB565 conversion failed" << std::endl;
        return false;
    }
    return true;
}

bool C2PImage::Write(const char* output_filename) {
    std::ofstream output_stream;
    return true;
}

bool C2PImage::ConvertRGB565() {
   //do test of if bytes are correct numbers eg. 0x1F, 0x3F 
   return true;
}

bool C2PImage::CompressData() {

}

bool C2PImage::CreateHeader() {
    
}

bool C2PImage::CreateFooter() {
    
}

C2PImage::~C2PImage() {
    
}