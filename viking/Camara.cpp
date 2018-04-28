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

// Vel. de camara 
#define velCam	0.1f


// Constructor
Camara::Camara() {
	vector3f vZero = vector3f(0.0, 0.0, 0.0);		// Init Vec 0 0 0 for our position
	vector3f vView = vector3f(0.0, 1.0, 0.5);		// Init view vVec (looking up and out the screen) 
	vector3f vUp   = vector3f(0.0, 0.0, 1.0);		// Init up vVec (Rarely ever changes)

	m_vPos	  = vZero;		
	m_vView	  = vView;		
	m_vUpVec  = vUp;		
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
void Camara::VistaMouse() {
    POINT mousePos;					 // Guarda pos X y Y del mouse
	int mitX = ancho  >> 1;	     // Corrimiento binario para obtener la mitad de ancho
	int mitY = alto >> 1;	     // Corrimiento binario para obtener la mitad de alto
	float anguloY = 0.0f;			 // Direccion vista arriba o abajo
	float anguloZ = 0.0f;			 // Rotacin en Y (Izquierda y Derecha)
	static float currentRotX = 0.0f;
	
	GetCursorPos(&mousePos); // Obtener pos actual del mouse						
	
	// Si el cursor esta en la mitad no nos movemos
	if((mousePos.x == mitX) && (mousePos.y == mitY)) return;

	SetCursorPos(mitX, mitY); // Poner cursor enmedio de la pantalla							

	// Obtener la direccion del mov del mouse, decrementar el numero bastante (/1000)
	anguloY = (float)((mitX - mousePos.x)) / 1000.0f;		
	anguloZ = (float)((mitY - mousePos.y)) / 1000.0f;		

    static float lastRotX = 0.0f; 
 	lastRotX = currentRotX; // Guardamos currentRotX 
	
	currentRotX += anguloZ;	// Rotation actual, restringimos vuelta de 360 en camara
 
	// Si la rotacion actual (radianes) es mayor a 1.0, la mantenemos (cap it).
	if(currentRotX > 1.0f) {
        currentRotX = 1.0f;
		if(lastRotX != 1.0f) {	// Rotar por el angulo restante 
            // Obtenemos eje de rotacion
			vector3f vAxis = Cruzado(m_vView - m_vPos, m_vUpVec);
			vAxis = Normalizar(vAxis);
			// Rotamos camara por el angulo restante (1.0f - lastRotX)
			RotarVista(1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	// Si la rotacion es menor a -1.0, no continuar
	else if(currentRotX < -1.0f) {
        currentRotX = -1.0f;
		if(lastRotX != -1.0f) {	// Rotar por el angulo restante
			vector3f vAxis = Cruzado(m_vView - m_vPos, m_vUpVec);
			vAxis = Normalizar(vAxis);
			// Rotamos ( -1.0f - lastRotX)
			RotarVista(-1.0f - lastRotX, vAxis.x, vAxis.y, vAxis.z);
		}
	}
	else { // Rotamos en cualquier direccin
        vector3f vAxis = Cruzado(m_vView - m_vPos, m_vUpVec);
		vAxis = Normalizar(vAxis);
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
	float cosTheta = (float)cos(angulo);
	float senTheta = (float)sin(angulo);
	// Encontramos la nueva pos de x para el nuevo punto rotado
	vNewView.x  = (cosTheta + (1 - cosTheta) * x * x)		* vView.x;
	vNewView.x += ((1 - cosTheta) * x * y - z * senTheta)	* vView.y;
	vNewView.x += ((1 - cosTheta) * x * z + y * senTheta)	* vView.z;
	// Encontramos la nueva pos de y para el nuevo punto rotado
	vNewView.y  = ((1 - cosTheta) * x * y + z * senTheta)	* vView.x;
	vNewView.y += (cosTheta + (1 - cosTheta) * y * y)		* vView.y;
	vNewView.y += ((1 - cosTheta) * y * z - x * senTheta)	* vView.z;
	// Encontramos la nueva pos de z para el nuevo punto rotado
	vNewView.z  = ((1 - cosTheta) * x * z - y * senTheta)	* vView.x;
	vNewView.z += ((1 - cosTheta) * y * z + x * senTheta)	* vView.y;
	vNewView.z += (cosTheta + (1 - cosTheta) * z * z)		* vView.z;

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
	vVector = Normalizar(vVector);
	
	m_vPos.x += vVector.x * vel;		
	m_vPos.z += vVector.z * vel;		
	m_vView.x += vVector.x * vel;		
	m_vView.z += vVector.z * vel;		
}

// Checamos movimiento con el teclado
void Camara::ChecarMov() {
	if(GetKeyState(VK_UP) & 0x80 || GetKeyState('W') & 0x80) {				
		MovCam(velCam);				
	}
	if(GetKeyState(VK_DOWN) & 0x80 || GetKeyState('S') & 0x80) {			
		MovCam(-velCam);				
	}
	if(GetKeyState(VK_LEFT) & 0x80 || GetKeyState('A') & 0x80) {			
		StrafeCam(-velCam);
	}
	if(GetKeyState(VK_RIGHT) & 0x80 || GetKeyState('D') & 0x80) {			
		StrafeCam(velCam);
	}	
}

// Actualizamos el movimiento de la camara 
void Camara::Actualizar() {
	vector3f vCruz = Cruzado(m_vView - m_vPos, m_vUpVec);
	m_vStrafe = Normalizar(vCruz); // Normalizamos el vector strafe

	VistaMouse();

	ChecarMov();	// Checamos movimiento con el teclado
}

// Posicionamos la camara en opengl
void Camara::Vista() {
    gluLookAt(m_vPos.x, m_vPos.y, m_vPos.z, m_vView.x, m_vView.y, m_vView.z,	
			  m_vUpVec.x, m_vUpVec.y, m_vUpVec.z);
}