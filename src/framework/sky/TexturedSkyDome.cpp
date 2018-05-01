/**
* @file TexturedSkyDome.cpp
* @brief Implementacin de la clase TexturedSkyDome
* @date Saturday, August 11, 2007 10:24:46 PM
*/

#include "TexturedSkyDome.h"
#include <GL/glut.h>

/**
*@copydoc SkyDome::SkyDome(float r)
*@brief Inicializa la textura
*@param id_textura El identificador de la textura
*/
TexturedSkyDome::TexturedSkyDome(float r, unsigned int id_textura) : SkyDome(r), m_textura(id_textura) {
    m_v_tile = 1.0F;
    m_h_tile = 1.0F;

    calculaUV();
}


/**
*@copydoc SkyDome::SkyDome(float dphi,float dtheta,float r)
*@brief Inicializa la textura
*@param id_textura El identificador de la textura
*/
TexturedSkyDome::TexturedSkyDome(float dphi, float dtheta, float r, unsigned int id_textura) :
        SkyDome(dphi, dtheta, r), m_textura(id_textura) {
    m_v_tile = 1.0F;
    m_h_tile = 1.0F;

    calculaUV();
}

/**
@brief Destructor de la clase
*/
TexturedSkyDome::~TexturedSkyDome() {

}

/**
*@brief Esta funcin coloca una textura en el domo
*@param id Es un identificador de textura (debe ser generado por openGL)
*/
void TexturedSkyDome::setTexture(unsigned int id) {
    m_textura = id;
}

/**
* @brief Renderea el Domo texturizado
* @author Fernando Montes de Oca Cespedes
* @date Friday, October 19, 2007 8:44:26 PM
* @param poscam vector de la camara
*/
void TexturedSkyDome::renderDome(vector3f poscam) {

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glBindTexture(GL_TEXTURE_2D, m_textura);

    int i = 0;
    glPushMatrix();
    glTranslatef(poscam.x, poscam.y, poscam.z);
    glRotatef(-90.0F, 1.0, 0.0, 0.0F);
    glBegin(GL_TRIANGLE_STRIP);
    for (i = 0; i < m_num_vert; i++) {
        glTexCoord2f(m_vrtx[i].uv.x, m_vrtx[i].uv.y);
        glVertex3f(m_vrtx[i].pos.x, m_vrtx[i].pos.y, m_vrtx[i].pos.z);
    }
    glEnd();
    glPopMatrix();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}


/**
*@copydoc Sky::render(vector3f poscam)
*@brief Renderea todos los elementos de un TexturedSkyDome
*/
void TexturedSkyDome::render(vector3f poscam) {
    renderDome(poscam);
    renderSun(poscam);
}

/**
*@copydoc Sky::actualiza(vector3f poscam)
*@brief Actualiza los elementos del TexturedSkyDome
*/
void TexturedSkyDome::actualiza(vector3f poscam) {
    render(poscam);
}

void TexturedSkyDome::calculaUV() {

    vector3f vec_temp;

    int i = 0;
    int phi = 0;
    int theta = 0;

    for (; phi <= 90 - m_dphi; phi += (int) m_dphi) {
        for (theta = 0; theta <= 360 - m_dtheta; theta += (int) m_dtheta) {

            vec_temp.x = m_vrtx[i].pos.x;
            vec_temp.y = m_vrtx[i].pos.y;
            vec_temp.z = m_vrtx[i].pos.z;

            Normalizar(vec_temp);

            // Calculo de las coordenadas de la textura en la esfera
            m_vrtx[i].uv.x = m_h_tile * (float) (atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            m_vrtx[i].uv.y = m_v_tile * (float) (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            vec_temp.x = m_vrtx[i].pos.x;
            vec_temp.y = m_vrtx[i].pos.y;
            vec_temp.z = m_vrtx[i].pos.z;

            Normalizar(vec_temp);

            // Calculo de las coordenadas de la textura en la esfera
            m_vrtx[i].uv.x = m_h_tile * (float) (atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            m_vrtx[i].uv.y = m_v_tile * (float) (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            vec_temp.x = m_vrtx[i].pos.x;
            vec_temp.y = m_vrtx[i].pos.y;
            vec_temp.z = m_vrtx[i].pos.z;

            Normalizar(vec_temp);

            // Calculo de las coordenadas de la textura en la esfera
            m_vrtx[i].uv.x = m_h_tile * (float) (atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
            m_vrtx[i].uv.y = m_v_tile * (float) (asinf(vec_temp.y) / PI) + 0.5F;
            i++;

            if (phi > -90 && phi < 90) {
                vec_temp.x = m_vrtx[i].pos.x;
                vec_temp.y = m_vrtx[i].pos.y;
                vec_temp.z = m_vrtx[i].pos.z;

                Normalizar(vec_temp);

                // Calculo de las coordenadas de la textura en la esfera
                m_vrtx[i].uv.x = m_h_tile * (float) (atan2(vec_temp.x, vec_temp.z) / (PI * 2)) + 0.5F;
                m_vrtx[i].uv.y = m_v_tile * (float) (asinf(vec_temp.y) / PI) + 0.5F;
                i++;
            }
        }
    }

    for (int j = 0; j < m_num_vert - 3; j++) {

        if (m_vrtx[j].uv.x - m_vrtx[j + 1].uv.x > 0.9F)
            m_vrtx[j + 1].uv.x += 1.0F;

        if (m_vrtx[j + 1].uv.x - m_vrtx[j].uv.x > 0.9F)
            m_vrtx[j].uv.x += 1.0F;

        if (m_vrtx[j].uv.x - m_vrtx[j + 2].uv.x > 0.9F)
            m_vrtx[j + 2].uv.x += 1.0F;

        if (m_vrtx[j + 2].uv.x - m_vrtx[j].uv.x > 0.9F)
            m_vrtx[j].uv.x += 1.0F;

        if (m_vrtx[j + 1].uv.x - m_vrtx[j + 2].uv.x > 0.9F)
            m_vrtx[j + 2].uv.x += 1.0F;

        if (m_vrtx[j + 2].uv.x - m_vrtx[j + 1].uv.x > 0.9F)
            m_vrtx[j + 1].uv.x += 1.0F;

        if (m_vrtx[j].uv.y - m_vrtx[j + 1].uv.y > 0.8F)
            m_vrtx[j + 1].uv.y += 1.0F;

        if (m_vrtx[j + 1].uv.y - m_vrtx[j].uv.y > 0.8F)
            m_vrtx[j].uv.y += 1.0F;

        if (m_vrtx[j].uv.y - m_vrtx[j + 2].uv.y > 0.8F)
            m_vrtx[j + 2].uv.y += 1.0F;

        if (m_vrtx[j + 2].uv.y - m_vrtx[j].uv.y > 0.8F)
            m_vrtx[j].uv.y += 1.0F;

        if (m_vrtx[j + 1].uv.y - m_vrtx[j + 2].uv.y > 0.8F)
            m_vrtx[j + 2].uv.y += 1.0F;

        if (m_vrtx[j + 2].uv.y - m_vrtx[j + 1].uv.y > 0.8F)
            m_vrtx[j + 1].uv.y += 1.0F;
    }
}