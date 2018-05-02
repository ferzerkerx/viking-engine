#ifndef __TEXTUREWRITER__H__
#define __TEXTUREWRITER__H__

#include <fstream>

/**
* @class TextureWriter
* @author Fernando Montes de Oca Cespedes
* @date Saturday, October 20, 2007 4:47:49 PM
*/
class TextureWriter {
private:
    std::ofstream out_;

protected:

    std::ofstream &out();

    virtual ~TextureWriter();

public:
    virtual void WriteTexture() = 0;

    explicit TextureWriter(const char *file_name);

};

#endif