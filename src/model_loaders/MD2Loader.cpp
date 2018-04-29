/**
* @file MD2Loader.cpp
* @brief Implementacin de la clase MD2
* @date Thursday, November 01, 2007 7:42:08 AM
*/

#include <cstring>
#include "MD2Loader.h"
#include "../Textura.h"
#include "../loggers/EventLogger.h"


MD2Loader::MD2Loader(MD2Model *model) : Model3DLoader(model) {
    m_pSkins = NULL;
    m_pTexCoords = NULL;
    m_pTriangles = NULL;
    m_pFrames = NULL;

    m_file = NULL;


}

MD2Loader::~MD2Loader() {

}

void MD2Loader::importar(const char *modelo) {
    importar(modelo, NULL);
}

void MD2Loader::importar(const char *modelo, const char *textura) {
    FN("MD2::readMD2File(char * modelo)");
    if (!modelo || !m_mdl) { return; }

    m_file = fopen(modelo, "rb");

    if (!m_file) {
        LOG("No existe el archivo: %s", modelo);
        return;
    }
    struct md2_header m_header;

    fread(&m_header, 1, sizeof(struct md2_header), m_file);
    LOG("ident = %d", m_header.ident);
    LOG("version = %d", m_header.version);
    LOG("skinwidth = %d", m_header.skinwidth);
    LOG("skinheight = %d", m_header.skinheight);
    LOG("framesize = %d", m_header.framesize);
    LOG("num_skins = %d", m_header.num_skins);
    LOG("num_vertices = %d", m_header.num_vertices);
    LOG("num_st = %d", m_header.num_st);
    LOG("num_tris = %d", m_header.num_tris);
    LOG("num_glcmds = %d", m_header.num_glcmds);
    LOG("num_frames = %d", m_header.num_frames);
    LOG("offset_skins = %d", m_header.offset_skins);
    LOG("offset_st = %d", m_header.offset_st);
    LOG("offset_tris = %d", m_header.offset_tris);
    LOG("offset_frames = %d", m_header.offset_frames);
    LOG("offset_glcmds = %d", m_header.offset_glcmds);
    LOG("offset_end = %d", m_header.offset_end);

    if (m_header.ident != IDENT || m_header.version != VERSION) {
        LOG("Error al cargar \"%s\" version de MD2 incorrecta", modelo);
        return;
    }

    leeMD2Data(m_header);
    convertDataStructures(m_header);

    if (textura) {
        Material texture;
        strcpy(texture.nombre, textura);
        Textura t;
        unsigned int tex[1];
        t.CrearTextura(tex, textura, 0);
        texture.texture_id = tex[0];
        texture.u_tile = texture.u_tile = 1;

        m_mdl->addMaterial(texture);
    }

    fclose(m_file);
    m_file = NULL;
    if (m_pSkins) { delete[] m_pSkins; }
    if (m_pTexCoords) { delete m_pTexCoords; }
    if (m_pTriangles) { delete m_pTriangles; }
    if (m_pFrames) { delete m_pFrames; }
    if (m_glCommands) { delete m_glCommands; }
}


void MD2Loader::leeMD2Data(md2_header &m_header) {
    // Create a larger buffer for the frames of animation (not fully used yet)
    unsigned char buffer[MD2_MAX_FRAMESIZE];
    int j = 0;

    // Here we allocate all of our memory from the header's information
    m_pSkins = new tMd2Skin[m_header.num_skins];
    m_pTexCoords = new tMd2TexCoord[m_header.num_st];
    m_pTriangles = new tMd2Face[m_header.num_tris];
    m_pFrames = new tMd2Frame[m_header.num_frames];


    fseek(m_file, m_header.offset_skins, SEEK_SET);
    fread(m_pSkins, sizeof(tMd2Skin), m_header.num_skins, m_file);


    fseek(m_file, m_header.offset_st, SEEK_SET);
    fread(m_pTexCoords, sizeof(tMd2TexCoord), m_header.num_st, m_file);


    fseek(m_file, m_header.offset_tris, SEEK_SET);
    fread(m_pTriangles, sizeof(tMd2Face), m_header.num_tris, m_file);

    fseek(m_file, m_header.offset_frames, SEEK_SET);
    tMd2AliasFrame *pFrame = (tMd2AliasFrame *) buffer;

    int i = 0;
    for (i = 0; i < m_header.num_frames; i++) {

        m_pFrames[i].pVertices = new tMd2Triangle[m_header.num_vertices];
        fread(pFrame, 1, m_header.framesize, m_file);
        strcpy(m_pFrames[i].strName, pFrame->name);


        tMd2Triangle *pVertices = m_pFrames[i].pVertices;

        for (j = 0; j < m_header.num_vertices; j++) {
            pVertices[j].vertex[0] = pFrame->aliasVertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
            pVertices[j].vertex[2] =
                    (pFrame->aliasVertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]) * -1;
            pVertices[j].vertex[1] = pFrame->aliasVertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
        }
    }

    m_glCommands = new int[m_header.num_glcmds];
    fseek(m_file, m_header.offset_glcmds, SEEK_SET);
    fread(m_glCommands, m_header.num_glcmds, sizeof(int), m_file);


}

void MD2Loader::convertDataStructures(md2_header &m_header) {
    int j = 0, i = 0;

    parseAnimations(m_header);

    for (i = 0; i < m_header.num_frames; i++) {
        Object3D currentFrame = {0};

        currentFrame.num_verts = m_header.num_vertices;
        currentFrame.num_st = m_header.num_st;
        currentFrame.num_faces = m_header.num_tris;

        // Allocate memory for the vertices, texture coordinates and face data.
        currentFrame.vertices = new vector3f[currentFrame.num_verts];

        // Go through all of the vertices and assign them over to our structure
        for (j = 0; j < currentFrame.num_verts; j++) {
            currentFrame.vertices[j].x = m_pFrames[i].pVertices[j].vertex[0];
            currentFrame.vertices[j].y = m_pFrames[i].pVertices[j].vertex[1];
            currentFrame.vertices[j].z = m_pFrames[i].pVertices[j].vertex[2];
        }

        delete m_pFrames[i].pVertices;

        if (i > 0) {
            m_mdl->addObject(currentFrame);
            continue;

        }

        currentFrame.text_st = new vector2f[currentFrame.num_st];
        // Go through all of the uv coordinates and assign them over to our structure.
        // The UV coordinates are not normal UV coordinates, they have a pixel ratio of
        // 0 to 256.  We want it to be a 0 to 1 ratio, so we divide the u value by the
        // skin width and the v value by the skin height.  This gives us our 0 to 1 ratio.
        // For some reason also, the v coordinate is flipped upside down.  We just subtract
        // the v coordinate from 1 to remedy this problem.
        for (j = 0; j < currentFrame.num_st; j++) {
            currentFrame.text_st[j].s = m_pTexCoords[j].u / float(m_header.skinwidth);
            currentFrame.text_st[j].t = 1 - m_pTexCoords[j].v / float(m_header.skinheight);
        }

        currentFrame.faces = new Face[currentFrame.num_faces];
        // Go through all of the face data and assign it over to OUR structure
        for (j = 0; j < currentFrame.num_faces; j++) {
            // Assign the vertex indices to our face data
            currentFrame.faces[j].vert_index[0] = m_pTriangles[j].vertexIndices[0];
            currentFrame.faces[j].vert_index[1] = m_pTriangles[j].vertexIndices[1];
            currentFrame.faces[j].vert_index[2] = m_pTriangles[j].vertexIndices[2];

            // Assign the texture coord indices to our face data
            currentFrame.faces[j].st_index[0] = m_pTriangles[j].textureIndices[0];
            currentFrame.faces[j].st_index[1] = m_pTriangles[j].textureIndices[1];
            currentFrame.faces[j].st_index[2] = m_pTriangles[j].textureIndices[2];
        }

        // Here we add the current object (or frame) to our list object list
        m_mdl->addObject(currentFrame);
    }

    ((MD2Model *) m_mdl)->setGlCommands(m_glCommands, m_header.num_glcmds);
}


void MD2Loader::parseAnimations(md2_header &m_header) {
    FN("MD2Loader::parseAnimations(md2_header  & m_header)");
    int i = 0, longitud = 0;
    int start = 0;
    char nombre[20] = {0};

    for (i = 0; i < m_header.num_frames; i++) {
        if (i == 0) {
            strcpy(nombre, m_pFrames[i].strName);
            longitud = strlen(nombre);
            dameNombreFrame(nombre, longitud);
            start = i;
            continue;
        }

        if ((longitud != strlen(m_pFrames[i].strName) || (strstr(m_pFrames[i].strName, nombre) == NULL))) {

            agregaAnimacion(start, i - 1, nombre);
            strcpy(nombre, m_pFrames[i].strName);
            longitud = strlen(nombre);
            dameNombreFrame(nombre, longitud);
            start = i;
        }
    }
    agregaAnimacion(start, i - 1, nombre);
}

void MD2Loader::agregaAnimacion(int start, int end, char *name) {
    Animacion anim = {0};
    anim.frame_inicial = start;
    anim.frame_final = end;
    strcpy(anim.nombre, name);
    ((MD2Model *) m_mdl)->addAnimation(anim);
    LOG("start= %d end= %d nombre = %s", anim.frame_inicial, anim.frame_final, anim.nombre);
}


void MD2Loader::dameNombreFrame(char *buff, int longitud) {
    if (buff[longitud - 2] == '0') { buff[longitud - 2] = '\0'; }
    else { buff[longitud - 1] = '\0'; }
}

