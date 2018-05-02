#include "BMPWriter.h"


BMPWriter::BMPWriter(const char *file, int width, int height, int prof, char *data) : TextureWriter(file) {
    if (prof < 3) { prof = 3; }

    file_header_.bfType = 19778;
    file_header_.bfSize = static_cast<unsigned int>(54 + (width * height * 3));//24 bits depth color
    file_header_.bfReserved1 = 0;
    file_header_.bfReserved2 = 0;
    file_header_.bfOffBits = 54;

    info_header_.biSize = 40;
    info_header_.biWidth = width;
    info_header_.biHeight = height;
    info_header_.biPlanes = 0;
    info_header_.biBitCount = 24;//bits por pixel
    info_header_.biHeight = height;
    info_header_.biCompression = 0;
    info_header_.biSizeImage = width * height * 3;
    info_header_.biXPelsPerMeter = 0;
    info_header_.biYPelsPerMeter = 0;
    info_header_.biClrUsed = 0;
    info_header_.biClrImportant = 0;

    data_ = data;
    color_depth_ = prof;
}

BMPWriter::~BMPWriter() {
    data_ = nullptr;
}

void BMPWriter::WriteTexture() {
    WriteFileHeader();
    WriteInfoHeader();
    WriteData();
}

void BMPWriter::WriteFileHeader() {
    std::ofstream &ofstream = out();
    ofstream.write((char *) &file_header_, sizeof(struct BITMAPFILEHEADER));
    ofstream.write((char *) &file_header_.bfType, 2);
    ofstream.write((char *) &file_header_.bfSize, 4);
    ofstream.write((char *) &file_header_.bfReserved1, 2);
    ofstream.write((char *) &file_header_.bfReserved2, 2);
    ofstream.write((char *) &file_header_.bfOffBits, 4);

}


void BMPWriter::WriteInfoHeader() {
    out().write((char *) &info_header_, sizeof(struct BITMAPINFOHEADER));
}


void BMPWriter::WriteData() {
    int offset = 0;
    int i = 0;
    int j = 0;

    int r = 0;
    int g = 0;
    int b = 0;

    std::ofstream &ofstream = out();

    for (i = 0; i < info_header_.biWidth; i++) {
        for (j = 0; j < info_header_.biHeight; j++) {

            if (data_ != nullptr) {
                offset = static_cast<int>(i * (info_header_.biHeight * color_depth_) + (j * color_depth_));
                ofstream.write(&data_[offset + 1], 1);//b
                ofstream.write(&data_[offset], 1);//g
                ofstream.write(&data_[offset + 2], 1);//r
            } else {
                ofstream.write((char *) &b, 1);//b
                ofstream.write((char *) &g, 1);//g
                ofstream.write((char *) &r, 1);//r
            }
        }
    }
}