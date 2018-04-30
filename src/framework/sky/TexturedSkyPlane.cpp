#include "TexturedSkyPlane.h"
#include <GL/glut.h>

/**
 * @brief Constructor del TexturedSkyPlane
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 9:55:51 PM
 * @param divisiones La cantidad de divisiones que se quieren ( >= 1 <= 256)
 * @param planet_radius El radio planetario del skyplane 
 * @param atmosphere_radius El Radio atmosfrico del skyplane
 * @param h_tile Escalamiento horizontal de textura
 * @param v_tile Escalamiento vertical de textura
 * @param texture identificador de la textura
 */
TexturedSkyPlane::TexturedSkyPlane(int divisiones, float planet_radius, float atmosphere_radius, float h_tile,
                                   float v_tile, unsigned int texture) {
    m_divisiones = static_cast<unsigned int>(divisiones);
    if (m_divisiones < 1)
        m_divisiones = 1;
    if (m_divisiones > 256)
        m_divisiones = 256;

    m_texture = texture;

    m_planet_radius = planet_radius;
    m_atmosphere_radius = atmosphere_radius;
    m_num_vertices = (m_divisiones + 1) * (m_divisiones + 1);
    m_num_indices = m_divisiones * m_divisiones * 2 * 3;
    m_indices = new unsigned int[m_num_indices];
    m_vertices = new vertx[m_num_vertices];
    m_roty_factor = 0.006f;

    m_h_tile = h_tile;
    m_v_tile = v_tile;

    m_wind_factor_x = 0.0001f;
    m_wind_factor_y = 0.0001f;
    m_desface_text_x = 0.0F;
    m_desface_text_y = 0.0F;
    m_rot_text_milli = 10;

    generaSkyPlane();
    printf("Hay %d poligonos en el Skyplane\n", m_num_indices / 3);
}


TexturedSkyPlane::~TexturedSkyPlane() {
    delete[] m_indices;
    delete[] m_vertices;
}


void TexturedSkyPlane::actualiza(vector3f poscam) {
    render(poscam);
}

/**
 * @copydoc Sky::render(vector3f poscam)
 * @brief Renderea los vertices del skyplano
 */
void TexturedSkyPlane::render(vector3f poscam) {
    //glMatrixMode(GL_MODELVIEW);
    if (m_mov_timer.getMilliseconds() >= m_rot_text_milli) {
        m_mov_timer.reset();
        m_desface_text_x += m_roty_factor;
        if (m_desface_text_x >= 360) {
            m_desface_text_x -= 360;
        }
    }

    glPushMatrix();
    glTranslatef(poscam.x, poscam.y + m_planet_radius, poscam.z);
    glRotatef(m_desface_text_x, 0.0, 1.0, 0.0);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    glBegin(GL_TRIANGLES);

    for (int i = 0; i < m_num_indices; i++) {
        glTexCoord2f(m_vertices[m_indices[i]].u, m_vertices[m_indices[i]].v);
        glVertex3f(m_vertices[m_indices[i]].pos.x, m_vertices[m_indices[i]].pos.y, m_vertices[m_indices[i]].pos.z);
    }

    glEnd();


    glDisable(GL_TEXTURE_2D);
    glDisable(GL_CULL_FACE);
    glDisable(GL_BLEND);
    glPopMatrix();
}


/**
*@brief Esta funcin genera el skyplane, calcula
* los vertices, uv's para texturizacin y los indices
* para el las divisiones y tamao especificados
*/
void TexturedSkyPlane::generaSkyPlane() {

    float plane_size = 2.0F * sqrt((SQR(m_atmosphere_radius) - SQR(m_planet_radius)));
    float delta = plane_size / (float) m_divisiones;
    float tex_delta = 2.0F / (float) m_divisiones;

    float x_dist;
    float z_dist;
    float x_height;
    float z_height;
    float height;

    int i = 0, j = 0;

    vertx sv; // temporary vertex

    for (i = 0; i <= m_divisiones; i++) {
        for (j = 0; j <= m_divisiones; j++) {
            x_dist = (-0.5f * plane_size) + ((float) j * delta);
            z_dist = (-0.5f * plane_size) + ((float) i * delta);

            x_height = (x_dist * x_dist) / m_atmosphere_radius;
            z_height = (z_dist * z_dist) / m_atmosphere_radius;
            height = x_height + z_height;

            sv.pos.x = x_dist;
            sv.pos.y = 0.0F - height;
            sv.pos.z = z_dist;

            // Calculate the texture coordinates
            sv.u = m_h_tile * ((float) j * tex_delta * 0.5f);
            sv.v = m_v_tile * (1.0F - (float) i * tex_delta * 0.5f);

            m_vertices[i * (m_divisiones + 1) + j] = sv;
        }
    }

    // Calculate the indices
    int index = 0;
    for (i = 0; i < m_divisiones; i++) {
        for (j = 0; j < m_divisiones; j++) {
            unsigned int startvert = (i * (m_divisiones + 1) + j);

            // tri 1f
            m_indices[index++] = startvert;
            m_indices[index++] = startvert + 1;
            m_indices[index++] = startvert + m_divisiones + 1;

            // tri 2
            m_indices[index++] = startvert + 1;
            m_indices[index++] = startvert + m_divisiones + 2;
            m_indices[index++] = startvert + m_divisiones + 1;
        }
    }
}

/**
*@brief Cambia el factor de viento en x
*@param wfx Es el delta de movimiento de la textura en x
*/
void TexturedSkyPlane::setWindFactorX(float wfx) {
    m_wind_factor_x = wfx;
}

/**
*@brief Cambia el factor de viento en y
*@param wfy Es el delta de movimiento de la textura en y
*/
void TexturedSkyPlane::setWindFactorY(float wfy) {
    m_wind_factor_y = wfy;
}

/**
*@brief Cambia el factor de viento en 'x' y 'y'
*@param wfx Es el delta de movimiento de la textura en x
*@param wfy Es el delta de movimiento de la textura en y
*/
void TexturedSkyPlane::setWindFactorXY(float wfx, float wfy) {
    m_wind_factor_x = wfx;
    m_wind_factor_y = wfy;
}