#include <iostream>
#include <memory>
#include "C2PImage.hpp"

int main() {
    std::string infile, outfile;
    std::cout << "<<<Img to C2P program>>>" << std::endl;

    std::cout << "Input image path: " << std::endl;
    std::getline(std::cin, infile);

    C2PImage* image = new C2PImage(infile);
    image->ConvertToC2P(); 

    std::cout << "Output filename: " << std::endl;
    std::getline(std::cin, outfile);
    if (!image->Write(outfile)) //if write does fail err
        std::cerr << "ERR: image write failed" << std::endl;

    delete image;
    return 0;
}