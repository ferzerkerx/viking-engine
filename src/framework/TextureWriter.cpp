#include "TextureWriter.h"


TextureWriter::TextureWriter(const char *file_name) {
    out_.open(file_name, std::ios::out | std::ios::binary);
    if (!out_) {
        throw std::invalid_argument("Could not open file\n");
    }
}

TextureWriter::~TextureWriter() {
    if (out_) {
        out_.close();
    }
}

std::ofstream &TextureWriter::out()  {
    return out_;
}
