#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb/stb_image_resize.h"

#include <string>
#include <iostream>
#include "Image.hpp"

#define JPG_QUALITY 100

// do i technically need all these functions? no
// am i removing them? no
Image::Image(std::string image_path, ImageFormat output_format)
 : m_image_data(NULL), m_output_format(output_format) {
    if (!Read(image_path, 0)) {
        std::cerr << "ERR: image load unsuccessful" << std::endl;
        exit(1);
    }
}

Image::Image(std::string image_path, int colour_channels, ImageFormat output_format)
 : m_image_data(NULL), m_output_format(output_format) {
    if (!Read(image_path, colour_channels)) {
        std::cerr << "ERR: image load unsuccessful" << std::endl;
        exit(1);
    }
    m_colour_channels = colour_channels; // since it was set manually
}


bool Image::Read(std::string image_path, int colour_channels) {
    //                                                                channels output     forced amt of channels
    m_image_data = stbi_load(image_path.c_str(), &m_width, &m_height, &m_colour_channels, colour_channels);
    // if image not loaded return false
    if(m_image_data == NULL) {
        std::cout << "null image" << std::endl;
        return false;
    }
    std::cout << "IMG: loaded image" << std::endl;
    return true;
}

bool Image::Resize(int req_width, int req_height) {
    int success = 0;
    int new_size = req_width * req_height * m_colour_channels;

    unsigned char* resized_image_data = (unsigned char*)malloc(new_size);
    success = stbir_resize_uint8(m_image_data, 
                                m_width, m_height,
                                0,                       // input stride
                                resized_image_data, 
                                req_width, req_height, 
                                0,                       // output stride
                                m_colour_channels);

    stbi_image_free(m_image_data);
    m_image_data = resized_image_data;
    resized_image_data = nullptr;

    m_width = req_width;
    m_height = req_height;

    std::cout << "IMG: resized image" << std::endl;
    return success;
}

bool Image::Write(std::string f) {
    int success = 0;
    std::cout << "IMG: exporting image" << std::endl;
    // do this because c strings are annoying
    std::string filename = f; 

    switch(m_output_format) {
        case ImageFormat::PNG:
            filename.append(".png");
            success = stbi_write_png(filename.c_str(), m_width, m_height, m_colour_channels, (void*)m_image_data, m_width * m_colour_channels);
            break;
        case ImageFormat::JPG:
            filename.append(".jpg");
            success = stbi_write_jpg(filename.c_str(), m_width, m_height, m_colour_channels, (void*)m_image_data, JPG_QUALITY);
            break;
        case ImageFormat::BMP:
            filename.append(".bmp");
            success = stbi_write_bmp(filename.c_str(), m_width, m_height, m_colour_channels, (void*)m_image_data);
            break;
    }
    return success;
}

Image::~Image() {
    stbi_image_free(m_image_data);
}