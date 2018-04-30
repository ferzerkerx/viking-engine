/**
* @file SkyBox.cpp
* @brief Implementacin de la clase SkyBox
* @date Saturday, October 20, 2007 1:27:58 PM
*/
#include "SkyBox.h"
#include <GL/glut.h>


/**
* @brief Genera una caja con un alto, largo, ancho especificos, y recibe las texturas en arreglo
* @warning Recibe un arreglo de identificadores de textura en este orden FRONT,BACK,LEFT,RIGHT,UP,DOWN
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 9:56:50 AM
* @param alto Alto de la caja, eje y en openGL
* @param largo Largo de la caja, eje x en openGL
* @param ancho Alto de la caja, eje y en openGL
* @param caras Arreglo de identificadores de textura en este orden FRONT,BACK,LEFT,RIGHT,UP,DOWN
*/
SkyBox::SkyBox(float alto, float largo, float ancho, unsigned int *caras) {
    if (alto <= 0.0F) { alto = 10.0F; }
    if (largo <= 0.0F) { largo = 10.0F; }
    if (ancho <= 0.0F) { ancho = 10.0F; }
    m_alto = alto / 2.0F;
    m_largo = largo / 2.0F;
    m_ancho = ancho / 2.0F;

    if (caras != nullptr) {
        for (int i = 0; i < 6; i++) {
            m_caras[i] = caras[i];
            glBindTexture(GL_TEXTURE_2D, m_caras[i]);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        }
    }
}

/**
* @brief Genera una caja con un alto, largo, ancho especificos, y recibe las texturas
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 9:56:55 AM
* @param alto Alto de la caja, eje y en openGL
* @param largo Largo de la caja, eje x en openGL
* @param ancho Alto de la caja, eje y en openGL
* @param front La cara frontal de la caja
* @param back La cara trasera de la caja
* @param left La cara izquierda de la caja
* @param right La cara derecha de la caja
* @param up La cara de arriba de la caja
* @param down La cara de abajo de la caja
*/
SkyBox::SkyBox(float alto, float largo, float ancho, unsigned int front,
               unsigned int back, unsigned int left, unsigned int right, unsigned int up, unsigned int down) {
    if (alto <= 0.0F) { alto = 10.0F; }
    if (largo <= 0.0F) { largo = 10.0F; }
    if (ancho <= 0.0F) { ancho = 10.0F; }
    m_alto = alto / 2.0F;
    m_largo = largo / 2.0F;
    m_ancho = ancho / 2.0F;

    m_caras[FRONT] = front;
    m_caras[BACK] = back;
    m_caras[LEFT] = left;
    m_caras[RIGHT] = right;
    m_caras[UP] = up;
    m_caras[DOWN] = down;

    for (unsigned int m_cara : m_caras) {
        glBindTexture(GL_TEXTURE_2D, m_cara);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
}

/**
* @brief Destructor del SkyBox libera las 6 texturas
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:18:55 PM
*/
SkyBox::~SkyBox() {
    glDeleteTextures(6, m_caras);
}

/**
* @brief Manda llamar a render
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:18:55 PM
* @param poscam Vector de la camara
*/
void SkyBox::actualiza(vector3f poscam) {
    render(poscam);
}

/**
* @brief Renderea las 6 caras del SkyBox activa culling para 
* GL_FRONT
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:19:55 PM
* @param poscam vector de la camara
*/
void SkyBox::render(vector3f poscam) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    glPushMatrix();
    glTranslatef(poscam.x, poscam.y, poscam.z);

    /****cara frontal****/
    glBindTexture(GL_TEXTURE_2D, m_caras[FRONT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(m_largo, -m_alto, -m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-m_largo, -m_alto, -m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-m_largo, m_alto, -m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(m_largo, m_alto, -m_ancho);
    glEnd();

    /****cara trasera****/
    glBindTexture(GL_TEXTURE_2D, m_caras[BACK]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(m_largo, m_alto, m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-m_largo, m_alto, m_ancho);
    glEnd();

    /****cara izquierda****/
    glBindTexture(GL_TEXTURE_2D, m_caras[LEFT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(-m_largo, -m_alto, -m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-m_largo, m_alto, m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(-m_largo, m_alto, -m_ancho);
    glEnd();

    /****cara derecha****/
    glBindTexture(GL_TEXTURE_2D, m_caras[RIGHT]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(m_largo, -m_alto, -m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(m_largo, m_alto, -m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(m_largo, m_alto, m_ancho);
    glEnd();

    /****cara arriba****/
    glBindTexture(GL_TEXTURE_2D, m_caras[UP]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(m_largo, m_alto, -m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-m_largo, m_alto, -m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-m_largo, m_alto, m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(m_largo, m_alto, m_ancho);
    glEnd();


    /****cara abajo****/
    glBindTexture(GL_TEXTURE_2D, m_caras[DOWN]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0F, 0.0F);
    glVertex3f(m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 0.0F);
    glVertex3f(-m_largo, -m_alto, m_ancho);
    glTexCoord2f(1.0F, 1.0F);
    glVertex3f(-m_largo, -m_alto, -m_ancho);
    glTexCoord2f(0.0F, 1.0F);
    glVertex3f(m_largo, -m_alto, -m_ancho);
    glEnd();
    glPopMatrix();

    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE_2D);
}

/**
* @brief Regresa el alto de la caja
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:29:03 PM
* @retval float  Alto de la caja
*/
float SkyBox::getAlto() {
    return m_alto * 2;
}

/**
* @brief Regresa el largo de la caja
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:29:03 PM
* @retval float  Largo de la caja
*/
float SkyBox::getLargo() {
    return m_largo * 2;
}

/**
* @brief Regresa el ancho de la caja
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 1:29:03 PM
* @retval float  ancho de la caja
*/
float SkyBox::getAncho() {
    return m_ancho * 2;
}