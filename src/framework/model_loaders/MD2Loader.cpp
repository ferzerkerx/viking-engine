#include <cstring>
#include <cstdio>
#include "MD2Loader.h"
#include "../loggers/EventLogger.h"


MD2Loader::MD2Loader() : Model3DLoader() {
    skins_ = nullptr;
    texture_coordinates_ = nullptr;
    faces_ = nullptr;
    frames_ = nullptr;

    file_ = nullptr;
}

Model3d *MD2Loader::Load(const char *md2_file_name) {
    return load(md2_file_name, nullptr);
}

Model3d *MD2Loader::load(const char *md2_file_name, const char *texture_file_name) {
    FN("MD2::readMD2File(char * md2_file_name)");
    if (!md2_file_name) {
        throw std::invalid_argument("No name was spacified.");
    }

    file_ = fopen(md2_file_name, "rb");

    if (!file_) {
        LOG("File not found: %s", md2_file_name);
        throw std::invalid_argument("File not found");
    }
    struct Md2Header m_header{};

    fread(&m_header, 1, sizeof(struct Md2Header), file_);
    LOG("ident = %d", m_header.ident);
    LOG("version = %d", m_header.version);
    LOG("skin_width = %d", m_header.skin_width);
    LOG("skin_height = %d", m_header.skin_height);
    LOG("frame_size = %d", m_header.frame_size);
    LOG("num_skins = %d", m_header.num_skins);
    LOG("num_vertices = %d", m_header.num_vertices);
    LOG("num_st = %d", m_header.num_st);
    LOG("num_tris = %d", m_header.num_tris);
    LOG("num_gl_commands = %d", m_header.num_gl_commands);
    LOG("num_frames = %d", m_header.num_frames);
    LOG("offset_skins = %d", m_header.offset_skins);
    LOG("offset_st = %d", m_header.offset_st);
    LOG("offset_tris = %d", m_header.offset_tris);
    LOG("offset_frames = %d", m_header.offset_frames);
    LOG("offset_gl_commands = %d", m_header.offset_gl_commands);
    LOG("offset_end = %d", m_header.offset_end);

    if (m_header.ident != IDENT || m_header.version != VERSION) {
        throw std::invalid_argument("Invalid md2 header");
    }

    ReadHeader(m_header);
    auto *md2Model = new Md2Model();
    PopulateModelInformation(m_header, md2Model);

    if (texture_file_name) {
        Material material;
        strcpy(material.name, texture_file_name);
        Texture t;
        unsigned int tex[1];
        t.LoadTexture(tex, texture_file_name, 0);
        material.texture_id = tex[0];
        material.u_tile = 1;
        material.v_tile = 1;

        md2Model->AddMaterial(material);
    }

    fclose(file_);
    file_ = nullptr;
    delete[] skins_;
    delete texture_coordinates_;
    delete faces_;
    delete frames_;
    delete glCommands_;


    md2Model->updateNormalVector();
    return md2Model;
}


void MD2Loader::ReadHeader(Md2Header &m_header) {
    unsigned char buffer[MD2_MAX_FRAMESIZE];
    int j = 0;

    skins_ = new Md2Skin[m_header.num_skins];
    texture_coordinates_ = new Md2TextureCoordinates[m_header.num_st];
    faces_ = new Md2Face[m_header.num_tris];
    frames_ = new Md2Frame[m_header.num_frames];


    fseek(file_, m_header.offset_skins, SEEK_SET);
    fread(skins_, sizeof(Md2Skin), static_cast<size_t>(m_header.num_skins), file_);


    fseek(file_, m_header.offset_st, SEEK_SET);
    fread(texture_coordinates_, sizeof(Md2TextureCoordinates), static_cast<size_t>(m_header.num_st), file_);


    fseek(file_, m_header.offset_tris, SEEK_SET);
    fread(faces_, sizeof(Md2Face), static_cast<size_t>(m_header.num_tris), file_);

    fseek(file_, m_header.offset_frames, SEEK_SET);
    auto *pFrame = (Md2AliasFrame *) buffer;

    int i = 0;
    for (i = 0; i < m_header.num_frames; i++) {

        frames_[i].pVertices = new Md2Triangle[m_header.num_vertices];
        fread(pFrame, 1, static_cast<size_t>(m_header.frame_size), file_);
        strcpy(frames_[i].strName, pFrame->name);


        Md2Triangle *pVertices = frames_[i].pVertices;

        for (j = 0; j < m_header.num_vertices; j++) {
            pVertices[j].vertex[0] = pFrame->alias_vertices[j].vertex[0] * pFrame->scale[0] + pFrame->translate[0];
            pVertices[j].vertex[2] =
                    (pFrame->alias_vertices[j].vertex[1] * pFrame->scale[1] + pFrame->translate[1]) * -1;
            pVertices[j].vertex[1] = pFrame->alias_vertices[j].vertex[2] * pFrame->scale[2] + pFrame->translate[2];
        }
    }

    glCommands_ = new int[m_header.num_gl_commands];
    fseek(file_, m_header.offset_gl_commands, SEEK_SET);
    fread(glCommands_, static_cast<size_t>(m_header.num_gl_commands), sizeof(int), file_);


}

void MD2Loader::PopulateModelInformation(Md2Header &md2_header, Md2Model *md2_Model) {
    int j = 0;
    int i = 0;

    ParseAnimations(md2_header, md2_Model);

    for (i = 0; i < md2_header.num_frames; i++) {
        Object3D current_object = {0};

        current_object.num_verts = md2_header.num_vertices;
        current_object.num_st = md2_header.num_st;
        current_object.num_faces = md2_header.num_tris;

        current_object.vertex = new vector3f[current_object.num_verts];

        for (j = 0; j < current_object.num_verts; j++) {
            current_object.vertex[j].x = frames_[i].pVertices[j].vertex[0];
            current_object.vertex[j].y = frames_[i].pVertices[j].vertex[1];
            current_object.vertex[j].z = frames_[i].pVertices[j].vertex[2];
        }

        delete frames_[i].pVertices;

        if (i > 0) {
            md2_Model->AddObject(current_object);
            continue;

        }

        current_object.texture_st = new vector2f[current_object.num_st];

        for (j = 0; j < current_object.num_st; j++) {
            current_object.texture_st[j].s = texture_coordinates_[j].u / float(md2_header.skin_width);
            current_object.texture_st[j].t = 1 - texture_coordinates_[j].v / float(md2_header.skin_height);
        }

        current_object.faces = new Face[current_object.num_faces];
        for (j = 0; j < current_object.num_faces; j++) {
            current_object.faces[j].vert_index[0] = faces_[j].vertex_indexes[0];
            current_object.faces[j].vert_index[1] = faces_[j].vertex_indexes[1];
            current_object.faces[j].vert_index[2] = faces_[j].vertex_indexes[2];

            current_object.faces[j].st_index[0] = faces_[j].texture_indexes[0];
            current_object.faces[j].st_index[1] = faces_[j].texture_indexes[1];
            current_object.faces[j].st_index[2] = faces_[j].texture_indexes[2];
        }

        md2_Model->AddObject(current_object);
    }

    md2_Model->set_GlCommands(glCommands_, md2_header.num_gl_commands);
}


void MD2Loader::ParseAnimations(Md2Header &md2_header, Md2Model *md2_Model) {
    FN("MD2Loader::ParseAnimations(Md2Header  & md2_header)");
    int i = 0;
    int name_size = 0;
    int start_of_frame = 0;
    char name[20] = {0};

    for (i = 0; i < md2_header.num_frames; i++) {
        if (i == 0) {
            strcpy(name, frames_[i].strName);
            name_size = static_cast<int>(strlen(name));
            FrameName(name, name_size);
            start_of_frame = i;
            continue;
        }

        if (name_size != strlen(frames_[i].strName) || (strstr(frames_[i].strName, name) == nullptr)) {
            AddAnimation(start_of_frame, i - 1, name);
            strcpy(name, frames_[i].strName);
            name_size = static_cast<int>(strlen(name));
            FrameName(name, name_size);
            start_of_frame = i;
        }
    }
    Animation animacion = AddAnimation(start_of_frame, i - 1, name);
    md2_Model->Add_animation(animacion);
}

Animation MD2Loader::AddAnimation(int start, int end, char *name) {
    Animation animation = {0};
    animation.initial_frame = start;
    animation.final_frame = end;
    strcpy(animation.name, name);
    return animation;

}


void MD2Loader::FrameName(char *buff, int length) {
    if (buff[length - 2] == '0') { buff[length - 2] = '\0'; }
    else { buff[length - 1] = '\0'; }
}

