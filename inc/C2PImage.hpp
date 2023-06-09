#pragma once
#include "Image.hpp"

class C2PImage : public Image {
    public:
        bool ConvertToC2P();
        bool Write(std::string f) override;

        C2PImage(std::string image_path);
        ~C2PImage() override;
    private:
        bool ConvertRGB565();
        bool CreateHeader(unsigned char* image_data);
        bool CreateFooter(unsigned char* image_data);
        bool CompressData();

        unsigned char* m_converted_image_data; /*image data after rgb565 and zlib compression
                                                 couldn't use m_image_data because can't include stb_image twice in this file
                                                 to free temp data, hopefully there is better solution*/
        unsigned char* m_formatted_data; //final image
        int m_compressed_data_size;
};