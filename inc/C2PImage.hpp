#pragma once
#include "Image.hpp"

class C2PImage : public Image {
    public:
        bool ConvertToC2P();
        bool Write(const char* output_filename) override; //return true if success

        C2PImage(const char* image_path, ImageFormat output_format);
        ~C2PImage() override;
    private:
        bool ConvertRGB565();
        bool CreateHeader();
        bool CreateFooter();
        bool CompressData();

        unsigned char* m_converted_image_data;
        unsigned char* m_formatted_image_data;
};