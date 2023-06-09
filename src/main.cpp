#include <iostream>
#include <memory>
#include "C2PImage.hpp"

int main() {
    std::string infile, outfile;
    bool keep_aspect_ratio;
    std::cout << "<<<Img to C2P program>>>" << std::endl;

    std::cout << "Type your image filename: " << std::endl;
    std::getline(std::cin, infile);

    C2PImage* image = new C2PImage(infile);

    std::cout << "Do you want to keep aspect ratio? Y/N" << std::endl;
    std::string answer;
    std::getline(std::cin, answer);

    std::string yes = "Yy";
    std::string no = "Nn";

    // cant use switch statement because retrieving 1 char from cin is pain
    if(answer.length() != 1) {
        std::cout << "incorrect argument, defaulting to true" << std::endl;
        keep_aspect_ratio = true;
    } else if(yes.find(answer) != std::string::npos) { // if answer is matched in yes string
        keep_aspect_ratio = true;
    } else if(no.find(answer)!= std::string::npos) {
        keep_aspect_ratio = false;
    } else {
        std::cout << "incorrect argument, defaulting to true" << std::endl;
        keep_aspect_ratio = true;
    }

    if(!image->ConvertToC2P(keep_aspect_ratio)) { // if conversion fails exit
        delete image;
        return 0;
    }

    std::cout << "Type your output filename: " << std::endl;
    std::getline(std::cin, outfile);
    image->Write(outfile);

    delete image;
    return 0;
}