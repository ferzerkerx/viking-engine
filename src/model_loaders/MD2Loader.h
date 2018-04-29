/**
* @file MD2Loader.h
* @brief Encabezado de la clase MD2
* @date Thursday, November 01, 2007 7:42:10 AM
*/

#ifndef __MD2_H__
#define __MD2_H__

#include "Model3DLoader.h"
#include "MD2Model.h"

#define IDENT 844121161
#define VERSION 8

#define MD2_MAX_VERTICES        2048
#define MD2_MAX_FRAMESIZE        (MD2_MAX_VERTICES * 4 + 128)


/**
* @class MD2Loader
* @author Fernando Montes de Oca Cspedes
* @date Thursday, November 29, 2007 8:54:42 PM
* @brief Clase que sabe leer y cargar un modelo .MD2
* 
*/
class MD2Loader : public Model3DLoader {

public:
    MD2Loader(MD2Model *model);

    ~MD2Loader();

    void importar(const char *modelo, const char *textura);

    void importar(const char *modelo);

private:

    struct tMd2AliasTriangle {
        unsigned char vertex[3]; //vertices de la cara comprimida
        unsigned char lightNormalIndex;    //indice de la normal de la cara comprimida
    };

    struct tMd2Triangle {
        float vertex[3]; //vertices
        float normal[3]; //normales
    };

    struct tMd2Face {
        short vertexIndices[3]; //indices de los vertices de la cara
        short textureIndices[3]; //indices de las coordenadas de la textura
    };

    struct tMd2TexCoord {
        short u; //coordenada u de la textura
        short v; //coordenada v de la textura
    };

    struct tMd2AliasFrame {
        float scale[3]; //escala del frame comprimido
        float translate[3];    //translacion del frame comprimido
        char name[16];    //nombre del frame comprimido
        tMd2AliasTriangle aliasVertices[1]; //cara comprimido
    };

    struct tMd2Frame {
        char strName[16];    //nombre del frame
        tMd2Triangle *pVertices;    //vertices del cuadro
    };


    typedef char tMd2Skin[64]; //Guarda los nombres de las texturas

    struct md2_header {
        int ident;             //numero magico: "IDP2"
        int version;           //version: debe ser 8
        int skinwidth;         //ancho de la textuta
        int skinheight;        //alto de la textura
        int framesize;         //tamao en bytes de un frame
        int num_skins;         //numero de texturas
        int num_vertices;      //numero de vertices por frame
        int num_st;            //numero de cooredenadas de textura
        int num_tris;          //numero de triangulos
        int num_glcmds;        //numero de comandos de OpenGL
        int num_frames;        //numero de frames
        int offset_skins;      //offset skin (texturas)
        int offset_st;         //offset cooredenadas de texturas
        int offset_tris;       //offset vertices
        int offset_frames;     //offset datos de cada frame
        int offset_glcmds;     //offset comandos de OpenGL
        int offset_end;        //offset EOF (final del archivo)
    };

    void leeMD2Data(md2_header &m_header);

    void convertDataStructures(md2_header &m_header);

    void parseAnimations(md2_header &m_header);

    void dameNombreFrame(char *buff, int longitud);

    void agregaAnimacion(int start, int end, char *name);

    tMd2Skin *m_pSkins;            // texturas usadas por el modelo
    tMd2TexCoord *m_pTexCoords;    // coordenadas de la textura
    tMd2Face *m_pTriangles;        // caras del modelo
    tMd2Frame *m_pFrames;        // The frames of animation (vertices)
    int *m_glCommands;            //comandos de openGL
    FILE *m_file;                //Apuntador al archivo
};


/**
* @var MD2Loader::::vertex
* @brief vertices de la cara comprimida
*/

/**
* @var MD2Loader::::lightNormalIndex
* @brief indice de la normal de la cara comprimida
*/

/**
* @var MD2Loader::tMd2Triangle::vertex
* @brief vertices
*/

/**
* @var MD2Loader::tMd2Triangle::normal
* @brief normales
*/

/**
* @var MD2Loader::tMd2Face::vertexIndices
* @brief indices de los vertices de la cara
*/

/**
* @var MD2Loader::tMd2Face::textureIndices
* @brief indices de las coordenadas de la textura
*/

/**
* @var MD2Loader::tMd2TexCoord::u
* @brief coordenada u de la textura
*/

/**
* @var MD2Loader::tMd2TexCoord::v
* @brief coordenada v de la textura
*/

/**
* @var MD2Loader::tMd2AliasFrame::scale
* @brief escala del frame comprimido
*/

/**
* @var MD2Loader::tMd2AliasFrame::translate
* @brief translacion del frame comprimido
*/

/**
* @var MD2Loader::tMd2AliasFrame::name
* @brief nombre del frame comprimido
*/

/**
* @var MD2Loader::tMd2AliasFrame::aliasVertices
* @brief cara comprimido
*/

/**
* @var MD2Loader::tMd2Frame::strName
* @brief nombre del frame
*/

/**
* @var MD2Loader::tMd2Frame::pVertices
* @brief vertices del cuadro
*/

/**
* @var MD2Loader::tMd2Skin
* @brief Guarda los nombres de las texturas
*/


/**
* @var MD2Loader::md2_header::ident
* @brief numero magico: "IDP2"
*/

/**
* @var MD2Loader::md2_header::version
* @brief version: debe ser 8
*/

/**
* @var MD2Loader::md2_header::skinwidth
* @brief ancho de la textuta
*/

/**
* @var MD2Loader::md2_header::skinheight
* @brief alto de la textura
*/

/**
* @var MD2Loader::md2_header::framesize
* @brief tamao en bytes de un frame
*/

/**
* @var MD2Loader::md2_header::num_skins
* @brief numero de texturas
*/

/**
* @var MD2Loader::md2_header::num_vertices
* @brief numero de vertices por frame
*/

/**
* @var MD2Loader::md2_header::num_st
* @brief numero de cooredenadas de textura
*/

/**
* @var MD2Loader::md2_header::num_tris
* @brief numero de triangulos
*/

/**
* @var MD2Loader::md2_header::num_glcmds
* @brief numero de comandos de OpenGL
*/

/**
* @var MD2Loader::md2_header::num_frames
* @brief numero de frames
*/

/**
* @var MD2Loader::md2_header::offset_skins
* @brief offset skin (texturas)
*/

/**
* @var MD2Loader::md2_header::offset_st
* @brief offset cooredenadas de texturas
*/

/**
* @var MD2Loader::md2_header::offset_tris
* @brief offset vertices
*/

/**
* @var MD2Loader::md2_header::offset_frames
* @brief offset datos de cada frame
*/

/**
* @var MD2Loader::md2_header::offset_glcmds
* @brief offset comandos de OpenGL
*/

/**
* @var MD2Loader::md2_header::offset_end
* @brief offset EOF (final del archivo)
*/

/**
* @var MD2Loader::m_pSkins
* @brief  texturas usadas por el modelo
*/

/**
* @var MD2Loader::m_pTexCoords
* @brief  coordenadas de la textura
*/

/**
* @var MD2Loader::m_pTriangles
* @brief  caras del modelo
*/

/**
* @var MD2Loader::m_pFrames
* @brief  The frames of animation (vertices)
*/

/**
* @var MD2Loader::m_glCommands
* @brief comandos de openGL
*/

/**
* @var MD2Loader::m_file
* @brief Apuntador al archivo
*/


#endif