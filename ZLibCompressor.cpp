#include <zlib/zlib.h>
#include "ZLibCompressor.hpp"

ZLibCompressor::ZLibCompressor(unsigned char* data, int data_size) : m_data(data), m_data_size(data_size) {

}

void ZLibCompressor::Compress() {
    long compressed_size
}



ZLibCompressor::~ZLibCompressor() {
    delete m_data;
}