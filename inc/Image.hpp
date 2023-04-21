enum ImageFormat {
    PNG, JPG, BMP
};

class Image {
    protected:
        int m_width, m_height, m_colour_channels;
        ImageFormat m_output_format;
        unsigned char *m_image_data;
    public:
        //return true if success
        bool Read(const char* image_path, int colour_channels);
        bool Resize(int req_width, int req_height);
        virtual bool Write(const char* output_filename);

        Image(const char* image_path, ImageFormat output_format);
        Image(const char* image_path, ImageFormat output_format, int colour_channels);
        virtual ~Image();
};