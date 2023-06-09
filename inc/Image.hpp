#pragma once

enum ImageFormat {
    PNG, JPG, BMP
};

class Image {
    public:
        //return true if success
        bool Read(std::string image_path, int colour_channels);
        bool Resize(int req_width, int req_height);
        virtual bool Write(std::string f);

        explicit Image(std::string image_path, ImageFormat output_format = ImageFormat::PNG);
        explicit Image(std::string image_path, int colour_channels, ImageFormat output_format = ImageFormat::PNG);
        virtual ~Image();
    protected:
        int m_width, m_height, m_colour_channels;
        unsigned char *m_image_data;
        ImageFormat m_output_format;
};