/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Diciembre 2006
 *
 * Header de la Clase que implementa la Camara First Person Shooter
 *
 ************************************************************************/

#ifndef _CAMERA_H
#define _CAMERA_H

#include "framework/Framework.h"


class Camara {
public:
    Camara();

    ~Camara() {};

    void PonCam(float posX, float posY, float posZ, float viewX, float viewY, float viewZ,
                float upVecX, float upVecY, float upVecZ);

    void RotarVista(float angulo, float X, float Y, float Z);

    void VistaMouse(int ancho, int alto, int mouseX, int mouseY);

    void StrafeCam(float vel);

    void MovCam(float vel);

    void ChecarMov(int key);   // Checa el movimiento de camara con el teclado
    void Actualizar();  // Se Update la camara cada frame
    void Vista();        // Implementa gluLookAt

    vector3f getPos() { return m_vPos; };

    vector3f getView() { return m_vView; };

private:
    vector3f m_vPos;
    vector3f m_vView;
    vector3f m_vUpVec;
    vector3f m_vStrafe;
};

#endif