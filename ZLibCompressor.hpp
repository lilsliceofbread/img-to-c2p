#pragma once

class ZLibCompressor {
    public:
        explicit ZLibCompressor(unsigned char* data, int data_size);
        ~ZLibCompressor();
        void Compress();
        unsigned char* GetData();
        int GetSize();
    private:
        int m_data_size;
        unsigned char m_data[];
};