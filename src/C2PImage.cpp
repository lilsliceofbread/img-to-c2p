#include "C2PImage.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "zlib/zlib.h"

#define MAX_WIDTH 0x136
#define MAX_HEIGHT 0x191
#define COLOUR_CHANNELS 3

#define HEADER_LENGTH 0xDC
#define FOOTER_LENGTH 0x17C
#define IMAGE_STARTOFFSET 0xDC

//putting this function here is a bit cringe but don't know where else
inline uint8_t ConvertBitRange(uint8_t num,uint8_t original_bit_max, uint8_t new_bit_max) {
    float scale_factor = static_cast<float>(original_bit_max / new_bit_max);
    uint8_t converted_num = static_cast<int>(num / scale_factor);
    return converted_num;
}

C2PImage::C2PImage(const char* image_path, ImageFormat output_format) 
: Image(image_path, COLOUR_CHANNELS, output_format), m_compressed_data_size(0) {
    std::cout << "c2pimage class constructed" << std::endl;

}

bool C2PImage::ConvertToC2P() {
    if(!Resize(MAX_WIDTH, MAX_HEIGHT)) {
        std::cerr << "ERR: image resize failed" << "\n";
        return false;
    }

    m_formatted_data = (unsigned char*)malloc(HEADER_LENGTH + (m_width * m_height * 2) + FOOTER_LENGTH);

    if(!ConvertRGB565()) {
        std::cerr << "ERR: image RGB565 conversion failed" << "\n";
        return false;
    }
    if(!CreateHeader(m_formatted_data)) {
        std::cerr << "ERR: c2pimage header creation failed" << "\n";
        return false;
    }   
    //appends converted image data to formatted image data as well
    if(!CompressData()) {
        std::cerr << "ERR: c2pimage compression failed" << "\n";
        return false;
    }
    if(!CreateFooter(m_formatted_data)) {
        std::cerr << "ERR: c2pimage footer creation failed" << std::endl;
        return false;
    } 
    return true;
}

bool C2PImage::Write(const char* output_filename) {
    std::ofstream output_stream;
    return true;

}

bool C2PImage::ConvertRGB565() {
    //unsafe af function, might be able to use shared_ptr
    int size = m_width * m_height * m_colour_channels;
    m_converted_image_data = (unsigned char*)malloc(size);
    uint8_t red, green, blue;
    uint16_t final_byte;
    unsigned char final_byte_1, final_byte_2;

    for(int i = 0; i < m_height; i++) {
        for(int j = 0; j < m_width; j++) {
            //i moves you down the rows, j moves you across the columns
            int current_index = i * m_width * m_colour_channels + j * m_colour_channels;

            //converted image data has 2 bytes for colour channels 
            int current_converted_index = i * m_width * 2 + j * 2;

            //check if index goes outside pointer
            if(current_index + 2 > size) {
                free(m_converted_image_data);
                return false;
            }

            red = m_image_data[current_index];
            green = m_image_data[current_index+1];
            blue = m_image_data[current_index+2];
            
            red = ConvertBitRange(red, 0xFF, 0x1F); //Between 0 and 31
            green = ConvertBitRange(green, 0xFF, 0x3F); //Between 0 and 63
            blue = ConvertBitRange(blue, 0xFF, 0x1F); //Between 0 and 31

            //bitshifting colour values into 2 bytes
            final_byte += red;
            final_byte <<= 11;
            final_byte += green;
            final_byte <<= 5;
            final_byte += blue;

            //converting to unsigned char for array
            final_byte_1 = static_cast<unsigned char>((final_byte >> 8) & 0xFF); //bitshift right by 1 byte
            final_byte_2 = static_cast<unsigned char>(final_byte & 0xFF); //0xFF ensures no numbers on leftmost byte (overflowing bytes?)

            //new colour channel bytes is 2
            m_converted_image_data[current_converted_index] = final_byte_1;
            m_converted_image_data[current_converted_index + 1] = final_byte_2;

            std::cout << m_converted_image_data[current_converted_index] << " "; //only shows half the bytes
            final_byte = 0;
            final_byte_1, final_byte_2 = 0;
            red, green, blue = 0;
        }
        std::cout << std::endl;
    }

    return true;
}

bool C2PImage::CompressData() {
    int success;
    int data_size = m_width * m_height * 2;
    int alloc_size = (data_size * 1.1) + 12;/*amount we will allocate for compression
                                            110% for nice safety and 12 bytes padding*/
    unsigned char* compressed_image_data = (unsigned char*)malloc(alloc_size);

    success = compress(compressed_image_data, (uLongf*)&alloc_size, m_converted_image_data, data_size);
    switch(success) {
        case Z_OK:
            std::cout << "compression success\n";
            break;
        case Z_MEM_ERROR:
            std::cerr << "ERR: ZLib ran out of memory\n";
            return false;
            break;
        case Z_BUF_ERROR:
            std::cerr << "ERR: ZLib output buffer too small\n";
            return false;
            break;
    }
    //append compressed_image_data to 
    m_compressed_data_size = alloc_size;
    return true;
}

bool C2PImage::CreateHeader(unsigned char* image_data) {   
    /*values from file format docs
      needs size of compressed data + header and footer length*/
    int _a;
    int _b;
    int _c;
    int _d;
    int _e;

    int _w = m_width;
    int _h = m_height;
    int _f;

    unsigned char header[] = {
        
    };

    if(!sizeof(header) == HEADER_LENGTH)
        return false;

    //if doesn't work check these values
    std::copy(header, header+HEADER_LENGTH, image_data);
    return true;
}

bool C2PImage::CreateFooter(unsigned char* image_data) {
    //assert correct length
    if(m_compressed_data_size == 0)
        return false;

    unsigned char footer[] = {
        0x30, 0x31, 0x30, 0x30, 0x00, 0x00, 0x00, 0x8C, 0x07, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x60, 0x00, 0x07, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x98, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x60, 0x00, 0x04, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x05, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x98, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x06, 0x28, 0x31, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x06, 0x28, 0x32, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x98, 0x00, 0x00, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF, 0x30, 0x31, 0x30, 0x30, 0xE0, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x02, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x02, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x03, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
		0x02, 0x50, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x02, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x01, 0x03, 0x14, 0x15, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00,
		0x03, 0x14, 0x15, 0x93, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x14, 0x15, 0x93,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x60, 0x00, 0x03, 0x14, 0x15, 0x93, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x10, 0x00, 0x01, 0x01, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
    };

    //can remove after first use
    if(!sizeof(footer) == FOOTER_LENGTH)
        return false;

    //if doesn't work check these values
    std::copy(footer, footer+FOOTER_LENGTH, image_data+HEADER_LENGTH+m_compressed_data_size-1);
    return true;
}

C2PImage::~C2PImage() {
    /*no idea if i should use free or delete[] here
      since i used malloc thought i would use c-style free*/
    free(m_converted_image_data);
    free(m_formatted_data);
}