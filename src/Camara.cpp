/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Diciembre 2006
 *
 * Clase que implementa la Camara First Person Shooter
 *
 ************************************************************************/

#include "Camara.h"
#include "GL/glut.h"

// Vel. de camara 
#define velCam    0.1f


Camara::Camara() {
    vector3f vZero = vector3f(0.0, 0.0, 0.0);        // Init Vec 0 0 0 for our position
    vector3f vView = vector3f(0.0, 1.0, 0.5);        // Init view vVec (looking up and out the screen)
    vector3f vUp = vector3f(0.0, 0.0, 1.0);        // Init up vVec (Rarely ever changes)

    m_vPos = vZero;
    m_vView = vView;
    m_vUpVec = vUp;
}

//	Asignar valores de la camara
void Camara::PonCam(float posX, float posY, float posZ,
                    float viewX, float viewY, float viewZ,
                    float upVecX, float upVecY, float upVecZ) {

    vector3f vPos = vector3f(posX, posY, posZ);
    vector3f vView = vector3f(viewX, viewY, viewZ);
    vector3f vUpVec = vector3f(upVecX, upVecY, upVecZ);

    m_vPos = vPos;
    m_vView = vView;
    m_vUpVec = vUpVec;
}

// Mover vista con el mouse
void Camara::VistaMouse(int ancho, int alto, int mouseX, int mouseY) {
    int mitX = ancho >> 1;         // Corrimiento binario para obtener la mitad de ancho
    int mitY = alto >> 1;         // Corrimiento binario para obtener la mitad de alto
    float anguloY;             // Direccion vista arriba o abajo
    float anguloZ;             // Rotacin en Y (Izquierda y Derecha)
    static float currentRotX = 0.0F;

    // Si el cursor esta en la mitad no nos movemos
    if ((mouseX == mitX) && (mouseY == mitY)) { return; }


    // Obtener la direccion del mov del mouse, decrementar el numero bastante (/1000)
    anguloY = (float) (mitX - mouseX) / 1000.0F;
    anguloZ = (float) (mitY - mouseY) / 1000.0F;

    static float lastRotX = 0.0F;
    lastRotX = currentRotX; // Guardamos currentRotX

    currentRotX += anguloZ;    // Rotation actual, restringimos vuelta de 360 en camara

    // Si la rotacion actual (radianes) es mayor a 1.0, la mantenemos (cap it).
    if (currentRotX > 1.0F) {
        currentRotX = 1.0F;
        if (lastRotX != 1.0F) {    // Rotar por el angulo restante
            // Obtenemos eje de rotacion
            vector3f vAxis = Cross(m_vView - m_vPos, m_vUpVec);
            vAxis = Normalize(vAxis);
            // Rotamos camara por el angulo restante (1.0F - lastRotX)
            RotarVista(1.0F - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    }
        // Si la rotacion es menor a -1.0, no continuar
    else if (currentRotX < -1.0F) {
        currentRotX = -1.0F;
        if (lastRotX != -1.0F) {    // Rotar por el angulo restante
            vector3f vAxis = Cross(m_vView - m_vPos, m_vUpVec);
            vAxis = Normalize(vAxis);
            // Rotamos ( -1.0F - lastRotX)
            RotarVista(-1.0F - lastRotX, vAxis.x, vAxis.y, vAxis.z);
        }
    } else { // Rotamos en cualquier direccin
        vector3f vAxis = Cross(m_vView - m_vPos, m_vUpVec);
        vAxis = Normalize(vAxis);
        // Rotar sobre el eje perpendicular
        RotarVista(anguloZ, vAxis.x, vAxis.y, vAxis.z);
    }
    // Siempre rotamos sobre eje Y
    RotarVista(anguloY, 0, 1, 0);
}

//	Rotar la vista 
void Camara::RotarVista(float angulo, float x, float y, float z) {
    vector3f vNewView;
    // Obtenemos vector vista
    vector3f vView = m_vView - m_vPos;
    // Calculamos el seno coseno del angulo una vez
    float cosTheta = cos(angulo);
    float senTheta = sin(angulo);
    // Encontramos la nueva pos de x para el nuevo punto rotado
    vNewView.x = (cosTheta + (1 - cosTheta) * x * x) * vView.x;
    vNewView.x += ((1 - cosTheta) * x * y - z * senTheta) * vView.y;
    vNewView.x += ((1 - cosTheta) * x * z + y * senTheta) * vView.z;
    // Encontramos la nueva pos de y para el nuevo punto rotado
    vNewView.y = ((1 - cosTheta) * x * y + z * senTheta) * vView.x;
    vNewView.y += (cosTheta + (1 - cosTheta) * y * y) * vView.y;
    vNewView.y += ((1 - cosTheta) * y * z - x * senTheta) * vView.z;
    // Encontramos la nueva pos de z para el nuevo punto rotado
    vNewView.z = ((1 - cosTheta) * x * z - y * senTheta) * vView.x;
    vNewView.z += ((1 - cosTheta) * y * z + x * senTheta) * vView.y;
    vNewView.z += (cosTheta + (1 - cosTheta) * z * z) * vView.z;

    // Aadimos el nuevo vector de rotacion a la pos para obtener la vista
    m_vView = m_vPos + vNewView;
}

// Movimientos laterales
void Camara::StrafeCam(float vel) {

    // Aadimos el vector strafe a nuestra pos
    m_vPos.x += m_vStrafe.x * vel;
    m_vPos.z += m_vStrafe.z * vel;

    // Aadimos el vector strafe a nuestra vista
    m_vView.x += m_vStrafe.x * vel;
    m_vView.z += m_vStrafe.z * vel;
}

// Movimientos frontales de la camara
void Camara::MovCam(float vel) {
    vector3f vVector = m_vView - m_vPos; // Obtener vector de vista actual

    // Normalizamos el vector de vista para tener la misma vel en todos los movs
    vVector = Normalize(vVector);

    m_vPos.x += vVector.x * vel;
    m_vPos.z += vVector.z * vel;
    m_vView.x += vVector.x * vel;
    m_vView.z += vVector.z * vel;
}

// Checamos movimiento con el teclado
void Camara::ChecarMov(int key) {
    switch (key) {
        case GLUT_KEY_DOWN:
            MovCam(-velCam);
            break;
        case GLUT_KEY_UP:
            MovCam(velCam);
            break;
        case GLUT_KEY_LEFT:
            StrafeCam(-velCam);
            break;
        case GLUT_KEY_RIGHT:
            StrafeCam(velCam);
            break;
        default:
            break;
    }
}

// Actualizamos el movimiento de la camara 
void Camara::Actualizar() {
    vector3f vCruz = Cross(m_vView - m_vPos, m_vUpVec);
    m_vStrafe = Normalize(vCruz); // Normalizamos el vector strafe
}

// Posicionamos la camara en opengl
void Camara::Vista() {
    gluLookAt(m_vPos.x, m_vPos.y, m_vPos.z, m_vView.x, m_vView.y, m_vView.z,
              m_vUpVec.x, m_vUpVec.y, m_vUpVec.z);
}