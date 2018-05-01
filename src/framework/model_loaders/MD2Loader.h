#ifndef __MD2_H__
#define __MD2_H__

#include "Model3DLoader.h"
#include "Md2Model.h"

#define IDENT 844121161
#define VERSION 8

#define MD2_MAX_VERTICES        2048
#define MD2_MAX_FRAMESIZE        (MD2_MAX_VERTICES * 4 + 128)


/**
* @class MD2Loader
* @author Fernando Montes de Oca Cespedes
* @date Thursday, November 29, 2007 8:54:42 PM
*
*/
class MD2Loader : public Model3DLoader {

public:
    MD2Loader();

    ~MD2Loader() override = default;

    Model3d *load(const char *md2_file_name, const char *texture_file_name);

    Model3d *Load(const char *md2_file_name) override;

private:

    struct Md2AliasTriangle {
        unsigned char vertex[3];
        unsigned char light_normal_index;
    };

    struct Md2Triangle {
        float vertex[3]; //vertex
        float normal[3]; //normal
    };

    struct Md2Face {
        short vertex_indexes[3];
        short texture_indexes[3];
    };

    struct Md2TextureCoordinates {
        short u;
        short v;
    };

    struct Md2AliasFrame {
        float scale[3];
        float translate[3];
        char name[16];
        Md2AliasTriangle alias_vertices[1];
    };

    struct Md2Frame {
        char strName[16];
        Md2Triangle *pVertices;
    };

    struct Md2Header {
        int ident;             //"IDP2"
        int version;           //version:8
        int skin_width;
        int skin_height;
        int frame_size;
        int num_skins;
        int num_vertices;
        int num_st;
        int num_tris;
        int num_gl_commands;
        int num_frames;
        int offset_skins;
        int offset_st;
        int offset_tris;
        int offset_frames;
        int offset_gl_commands;
        int offset_end;
    };

    typedef char Md2Skin[64];

    void ReadHeader(Md2Header &m_header);

    void PopulateModelInformation(Md2Header &md2_header, Md2Model *md2_Model);

    void ParseAnimations(Md2Header &md2_header, Md2Model *md2_Model);

    Animation AddAnimation(int start, int end, char *name);

    void FrameName(char *buff, int length);

    Md2Skin *skins_;
    Md2TextureCoordinates *texture_coordinates_;
    Md2Face *faces_;
    Md2Frame *frames_;
    int *glCommands_;
    FILE *file_;
};

#endif