#include <GL/gl.h>
#include "SkyDome.h"


/**
*@brief Constructor del domo que usa las divisiones predefinidas, genera el domo
*inicializa el objeto de nubes (Clouds), e inicializa la posicin del sol
*@param r es el radio de la esfera
*/
SkyDome::SkyDome(float r) {
    m_r = r;
    m_dphi = 5.0f;
    m_dtheta = 10.0f;
    m_has_sun_text = false;
    m_sun_text = -1;

    m_vrtx = NULL;
    generaHemiEsfera();
    m_sun_polar = polar3f(1, 0, m_r - 2);
}


/**
*@brief Constructor del domo que permite definir las divisiones deseadas, genera el domo
*inicializa el objeto de nubes (Clouds), e inicializa la posicin del sol
*@param r es el radio de la esfera
*@param dphi es el delta de phi y define cuantos poligonos tendra la hemiesfera
*@param dtheta es el delta de theta y define cuantos poligonos tendra la hemisefera
*/
SkyDome::SkyDome(float dphi, float dtheta, float r) {
    m_dphi = dphi;
    m_dtheta = dtheta;
    m_r = r;
    m_has_sun_text = false;
    m_sun_text = -1;

    m_vrtx = NULL;
    generaHemiEsfera();
    m_sun_polar = polar3f(0, 0, m_r - 5);
}

/**
@brief Destructor de la clase, libera los vertices alojados
*y destruye objeto de nubes (Clouds)
*/
SkyDome::~SkyDome() {
    if (m_vrtx != NULL) { delete[] m_vrtx; }
    if (m_has_sun_text) { glDeleteTextures(1, &m_sun_text); }
}

/**
*@brief Genera la hemiesfera que representa el SkyDome
*/
void SkyDome::generaHemiEsfera() {
    m_num_vert = (int) ((360.0f / m_dtheta) * (90.0f / m_dphi) *
                        4.0f);//el 4 es porque se va a dibujar usando GL_TRIANGLE_STRIP

    m_vrtx = new vertxDome[m_num_vert];

    int i = 0, phi = 0, theta = 0;
    float phirad = 0.0f, thetarad = 0.0f;
    float phidrad = 0.0f, thetadrad = 0.0f;

    for (; phi <= 90 - m_dphi; phi += (int) m_dphi) {
        for (theta = 0; theta <= 360 - m_dtheta; theta += (int) m_dtheta) {
            phirad = phi * RAD;
            thetarad = theta * RAD;
            phidrad = (phi + m_dphi) * RAD;
            thetadrad = (theta + m_dtheta) * RAD;

            /*primer punto segun el teorema**/
            m_vrtx[i].pos = vector3f(m_r * sinf(phirad) * cosf(thetarad),
                                     m_r * sinf(phirad) * sinf(thetarad),
                                     m_r * cosf(phirad));
            m_vrtx[i].pol = polar3f(phi, theta, m_r);
            i++;

            /*Segundo punto segun el teorema**/
            m_vrtx[i].pos = vector3f(m_r * sinf(phidrad) * cosf(thetarad),
                                     m_r * sinf(phidrad) * sinf(thetarad),
                                     m_r * cosf(phidrad));
            m_vrtx[i].pol = polar3f(phi + m_dphi, theta, m_r);
            i++;

            /*Tercer punto segun el teorema**/
            m_vrtx[i].pos = vector3f(m_r * sinf(phirad) * cosf(thetadrad),
                                     m_r * sinf(phirad) * sinf(thetadrad),
                                     m_r * cosf(phirad));
            m_vrtx[i].pol = polar3f(phi, theta + m_dtheta, m_r);
            i++;

            if (phi > -90 && phi < 90) {
                /*Cuarto punto segun el teorema**/
                m_vrtx[i].pos = vector3f(m_r * sinf(phidrad) * cosf(thetadrad),
                                         m_r * sinf(phidrad) * sinf(thetadrad),
                                         m_r * cosf(phidrad));
                m_vrtx[i].pol = polar3f(phi + m_dphi, theta + m_dtheta, m_r);
                i++;
            }
        }
    }
    printf("hay %d poligonos en la hemiesfera\n", i + 1);
}

/**
*@brief Esta funcin coloca una textura para dibujar el sol
*@param id Es un identificador de textura (debe ser generado por openGL)
*/
void SkyDome::setSunTexture(unsigned int id) {
    m_sun_text = id;
    m_has_sun_text = true;
}

/**
* @brief Renderea el sol si es que hay textura
* @author Fernando Montes de Oca Cspedes
* @date Friday, October 19, 2007 8:35:41 PM
* @param poscam Vector de posicion de la camara
*/
void SkyDome::renderSun(vector3f poscam) {
    if (m_has_sun_text) {
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);
        glEnable(GL_BLEND);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, m_sun_text);
        glPushMatrix();
        glTranslatef(poscam.x, poscam.y, poscam.z);
        m_sun_vector = vector3f(m_sun_polar);
        m_sun_polar.phi += 0.002f;
        glTranslatef(m_sun_vector.y, m_sun_vector.x, m_sun_vector.z);
        //printf("%f,%f,%f\n",m_sun_vector.y,m_sun_vector.x,m_sun_vector.z);
        glRotatef(-90.0f, 1.0, 0.0, 0.0f);
        Billboard::BBEsfera(poscam, m_sun_vector.y, m_sun_vector.x, m_sun_vector.z);
        //Billboard::BBFalsoEsfera();
        glBegin(GL_POLYGON);
        glTexCoord2f(0.0, 1.0);
        glVertex3f(-5.0, 0.0, -5.0);
        glTexCoord2f(1.0, 1.0);
        glVertex3f(5.0, 0.0, -5.0);
        glTexCoord2f(1.0, 0.0);
        glVertex3f(5.0, 0.0, 5.0);
        glTexCoord2f(0.0, 0.0);
        glVertex3f(-5.0, 0.0, 5.0);
        glEnd();
        Billboard::BBFin();
        glPopMatrix();
        glDisable(GL_BLEND);
        glDisable(GL_TEXTURE_2D);
    }
}