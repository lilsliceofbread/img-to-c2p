#pragma once
#include "Image.hpp"

class C2PImage : public Image {
    public:
        bool ConvertToC2P(bool keep_aspect_ratio);
        bool Write(std::string f);

        C2PImage(std::string image_path);
        ~C2PImage() override;
    private:
        bool ConvertRGB565();
        bool CompressData();
        bool CreateHeader(uint8_t* image_data);
        bool CreateFooter(uint8_t* image_data);

        uint8_t* m_formatted_data; // final image
        int m_compressed_data_size;
};