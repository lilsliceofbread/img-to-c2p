#include "Image.hpp"

class C2PImage : public Image {
    private:
        bool ConvertRGB565();
        bool CreateHeader();
        bool CreateFooter();
        bool CompressData();
    public:
        bool ConvertToC2P();
        bool Write(const char* output_filename) override; //return true if success

        C2PImage(const char* image_path, ImageFormat output_format, int colour_channels);
        ~C2PImage() override;
};