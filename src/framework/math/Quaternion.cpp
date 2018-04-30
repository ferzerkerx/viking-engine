/**
* @file Quaternion.cpp
* @brief Implementacin de la clase Quaternion
* @date Friday, October 26, 2007 8:07:09 PM
*/
#include <stdio.h>
#include <math.h>
#include "Quaternion.h"


/**
* @brief Constructor que crea un Quaternion unitario
* @author Fernando Montes de Oca Cspedes
* @date Sunday, October 28, 2007 11:58:10 PM
*/
Quaternion::Quaternion() {
    w = 1.0F;
    x = 0.0F;
    y = 0.0F;
    z = 0.0F;
}

/**
* @brief Inicializa un Quaternion con los componentes
* indicados
* @author Fernando Montes de Oca Cspedes
* @date Sunday, October 28, 2007 11:58:41 PM
* @param w Componente real del Quaternion
* @param x Componente imaginario i del Quaternion
* @param y Componente imaginario j del Quaternion
* @param z Componente imaginario k del Quaternion
*/
Quaternion::Quaternion(float w, float x, float y, float z) {
    this->w = w;
    this->x = x;
    this->y = y;
    this->z = z;
}

/**
* @brief Regresa el Quaternion correspondiente a una
* rotacion con angulo theta sobre el eje dado
* @author Fernando Montes de Oca Cspedes
* @date Sunday, October 28, 2007 11:59:59 PM
* @param theta Angulo de la rotacion
* @param eje Eje de rotacion
*/
Quaternion::Quaternion(float theta, vector3f *eje) {
    /**
    * q = cos(theta/2) + A sin(theta/2) donde:
    * - q es el Quaternion resultante
    * - A el eje de rotacion
    * - theta el angulo de rotacion
    **/
    if (theta > 360.0F) { theta = 360.0F; }
    if (theta < 0.0F) { theta = 0.0F; }
    w = cos(theta / 2.0F);
    x = eje->x * sin(theta / 2.0F);
    y = eje->y * sin(theta / 2.0F);
    z = eje->z * sin(theta / 2.0F);
}

/**
* @brief Crea un Quaternion a partir de un arreglo
* que representa una matriz
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:03:45 AM
* @param matriz Arreglo que representa una matriz de 3x3 o 4x4
* @param num_columnas Especifica si la matriz es de 3x3 o de 4x4
*/
Quaternion::Quaternion(float *matriz, int num_columnas) {

    if (!matriz || ((num_columnas != 3) && (num_columnas != 4))) return;

    float *m = matriz;
    float m4x4[16] = {0};

    /*Si la matriz es de 3x3 la cambiamos a una de 4x4 homogenea**/
    if (num_columnas == 3) {
        m4x4[0] = m[0];
        m4x4[1] = m[1];
        m4x4[2] = m[2];
        m4x4[4] = m[3];
        m4x4[5] = m[4];
        m4x4[6] = m[5];
        m4x4[8] = m[6];
        m4x4[9] = m[7];
        m4x4[10] = m[8];

        m4x4[15] = 1;

        m = &m4x4[0];
    }

    /*calculamos el trace que es la suma de los elementos de la diagonal*/
    float trace = m[0] + m[5] + m[10]; //t00 + t11 + t22

    /*variable que usamos para facilitar los calculos**/
    float escala = 0.0F;

    if (trace > 0.00000001) {/* |w|> 1/2 **/

        /*
        * w = sqrt(trace + 1.0F) * (1/2)
        * escala = 4w
        * x = (t12 - t21)/4w
        * y = (t20 - t02)/4w
        * z = (t01 - t10)/4w
        */
        w = sqrt(trace + 1.0F) * 0.5f;
        escala = 4 * w;
        x = (m[6] - m[9]) / escala;
        y = (m[8] - m[2]) / escala;
        z = (m[1] - m[4]) / escala;
    } else { /* |w|<= 1/2 **/

        /*El primer elemento de la diagonal es el mayor**/
        if (m[0] > m[5] && m[0] > m[10]) {

            /*
            * x = sqrt(t00 - t11 - t22 + 1 ) * (1/2)
            * escala = 4x
            * w = (t12 - t21)/4x
            * y = (t01 + t10)/4x
            * z = (t02 + t20)/4x
            */
            x = sqrt(1.0F + m[0] - m[5] - m[10]) * 0.5f;
            escala = 4 * x;
            w = (m[6] - m[9]) / escala;
            y = (m[1] + m[4]) / escala;
            z = (m[2] + m[8]) / escala;
        }
            /*El segundo elemento de la diagonal es el mayor**/
        else if (m[5] > m[10]) {
            /*
            * y = sqrt(t11 - t00 - t22 + 1 ) * (1/2)
            * escala = 4y
            * x = (t01 + t10)/4y
            * z = (t12 + t21)/4y
            * w = (t20 - t02)/4y
            */
            y = sqrt(1.0F + m[5] - m[0] - m[10]) * 0.5f;
            escala = 4 * y;
            x = (m[1] + m[4]) / escala;
            z = (m[6] + m[9]) / escala;
            w = (m[8] - m[2]) / escala;

        }
            /*El tercer elemento de la diagonal es el mayor**/
        else {

            /*
            * z = sqrt(t22 - t00 - t11 + 1 ) * (1/2)
            * escala = 4z
            * x = (t02 + t20)/4z
            * y = (t12 + t21)/4z
            * w = (t01 - t10)/4z
            */
            z = sqrt(1.0F + m[10] - m[0] - m[5]) * 0.5f;
            escala = 4 * z;
            x = (m[2] + m[8]) / escala;
            y = (m[6] + m[9]) / escala;
            w = (m[1] - m[4]) / escala;
        }
    }
}

/**
* @brief Compara si 2 Quaterniones son iguales
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:05:21 AM
* @retval bool verdadero si sus componentes son iguales, falso en caso contrario
* @param q El operando derecho de la comparacion
*/
bool Quaternion::operator==(const Quaternion &q) {
    if (w == q.w && x == q.x && y == q.y && z == q.z) { return true; }
    return false;
}

/**
* @brief Suma el Quaternion actual con otro y regresa
* un tercer Quaternion con el resultado de la suma
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval Quaternion Resultado de la suma
* @param q El operando derecho de la suma
*/
Quaternion Quaternion::operator+(const Quaternion &q) {
    return Quaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}

/**
* @brief Resta el Quaternion actual con otro y regresa
* un tercer Quaternion con el resultado de la resta
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval Quaternion Resultado de la resta
* @param q El operando derecho de la resta
*/
Quaternion Quaternion::operator-(const Quaternion &q) {
    return Quaternion(w - q.w, x - q.x, y - q.y, z - q.z);
}

/**
* @brief Multiplica el Quaternion actual con otro y regresa
* un tercer Quaternion con el resultado de la multiplicacion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval Quaternion Resultado de la multiplicacion
* @param q El operando derecho de la multiplicacion
*/
Quaternion Quaternion::operator*(const Quaternion &q) {
    return Quaternion(w * q.w - x * q.x - y * q.y - z * q.z,
                      w * q.x + x * q.w + y * q.z - z * q.y,
                      w * q.y - x * q.z + y * q.w + z * q.x,
                      w * q.z + x * q.y - y * q.x + z * q.w);
}

/**
* @brief Multiplica el Quaternion actual con un escalar y regresa
* un Quaternion con el resultado de la multiplicacion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval Quaternion Resultado de la multiplicacion
* @param f El escalar por el que se va a multiplicar el Quaternion
*/
Quaternion Quaternion::operator*(float f) {
    return Quaternion(w * f, x * f, y * f, z * f);
}

/**
* @copydoc Quaternion::rotaVector(vector3f p)
*/
vector3f Quaternion::operator*(vector3f p) {
    return rotaVector(p);
}

/**
* @brief Divide el Quaternion actual con un escalar y regresa
* un Quaternion con el resultado de la division
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval Quaternion Resultado de la division
* @param f El escalar por el que se va a dividir el Quaternion
*/
Quaternion Quaternion::operator/(float f) {
    if (f == 0.0F) { return Quaternion(0.0F, 0.0F, 0.0F, 0.0F); }
    return Quaternion(w / f, x / f, y / f, z / f);
}

/**
* @brief Suma el Quaternion actual por el Quaternion especificado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:35:29 AM
* @retval Quaternion El Quaternion actual despues de la suma
* @param q El Quaternion por el que se va a sumar el Quaternion actual
*/
Quaternion &Quaternion::operator+=(const Quaternion &q) {
    w += q.w;
    x += q.x;
    y += q.y;
    z += q.z;
    return *this;
}

/**
* @brief Resta el Quaternion actual por el Quaternion especificado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:35:29 AM
* @retval Quaternion El Quaternion actual despues de la resta
* @param q El Quaternion por el que se va a restar el Quaternion actual
*/
Quaternion &Quaternion::operator-=(const Quaternion &q) {
    w -= q.w;
    x -= q.x;
    y -= q.y;
    z -= q.z;
    return *this;
}

/**
* @brief Multiplica el Quaternion actual por el Quaternion especificado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:35:29 AM
* @retval Quaternion El Quaternion actual despues de la multiplicacion
* @param q El Quaternion por el que se va a multiplicar el Quaternion actual
*/
Quaternion &Quaternion::operator*=(const Quaternion &q) {
    w = w * q.w - x * q.x - y * q.y - z * q.z;
    x = w * q.x + x * q.w + y * q.z - z * q.y;
    y = w * q.y - x * q.z + y * q.w + z * q.x;
    z = w * q.z + x * q.y - y * q.x + z * q.w;
    return *this;
}

/**
* @brief Multiplica el Quaternion por el escalar especificado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:35:29 AM
* @retval Quaternion El Quaternion actual despues de la multiplicacion
* @param f Escalar por el que se va a multiplicar el Quaternion
*/
Quaternion &Quaternion::operator*=(float f) {
    w *= f;
    x *= f;
    y *= f;
    z *= f;
    return *this;
}

/**
* @brief Divide el Quaternion por el escalar especificado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:35:29 AM
* @retval Quaternion El Quaternion actual despues de la division
* @param f Escalar por el que se va a dividir el Quaternion
*/
Quaternion &Quaternion::operator/=(float f) {
    if (f != 0.0F) {
        w /= f;
        x /= f;
        y /= f;
        z /= f;
    }
    return *this;
}

/**
* @brief Multiplica el Quaternion actual con un vector
* el resultado de esto es una rotacion del vector
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:06:55 AM
* @retval vector3f El vector rotado por el Quaternion actual
* @param p El vector(punto) a rotar
*/
vector3f Quaternion::rotaVector(vector3f p) {
    /**
    * Se calcula Protado = qPq^1 donde:
    * - P es el punto a rotar
    * - q es un quaternion unitario
    * - Protado = qPq^1 = P * ( w*w - q_xyz.q_xyz) ) +  (2w * (q_xyz x P) + (2 * (q_xyz . P) * q_xyz)
    * dado que q es un Quaternion unitario podemos decir que w*w + x*x + y*y +z*z = 1 y dado que
    * q_xyz.q_xyz = x*x + y*y + z*z
    * si despejamos obtenemos:
    * - q_xyz.q_xyz= x*x + y*y +z*z = 1-w*w
    * substituyendo en la ecuacin original tenemos:
    * - Protado = qPq^1 = P * ( w*w - (1 -w*w)) +  (2w * (q_xyz x P) + (2 * (q_xyz . P) * q_xyz)
    * - Protado = qPq^1 = P * ( 2w*w - 1) +  (2w * (q_xyz x P) + (2 * (q_xyz . P) * q_xyz)
    **/

    //Tratamos x,y,z del quaternion como un vector
    vector3f q_xyz(x, y, z);

    //Calculamos la primera parte de la ecuacion: (P * ( 2w*w - 1))
    vector3f t1 = p * (2.0F * w * w - 1.0F);

    //Calculamos la segunda parte de la ecuacion: (2w * (q_xyz x P)
    vector3f t2 = Cruzado(q_xyz, p) * 2.0F * w;

    //Calculamos la tercer parte de la ecuacion: (2 * (q_xyz . P) * q_xyz)
    vector3f t3 = q_xyz * 2.0F * Punto(q_xyz, p);

    // obtenemos el Punto resultante
    return t1 + t2 + t3;
}

/**
* @brief Regresa un Quaternion nuevo que representa
* el Quaternion actual normalizado
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:15:02 AM
* @retval Quaternion Quaternion unitario (normalizado)
*/
Quaternion Quaternion::normaliza() {
    float mag = getMagnitud();
    return Quaternion(w / mag, x / mag, y / mag, z / mag);
}

/**
* @brief Regresa el conjugado de un Quaternion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:21:02 AM
* @retval Quaternion Quaternion que representa el conjugado del Quaternion actual
*/
Quaternion Quaternion::getConjugado() {
    return Quaternion(w, -x, -y, -z);
}

/**
* @brief Regresa la inversa de un Quaternion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:21:40 AM
* @retval Quaternion Quaternion que representa la inversa del quaternion actual
*/
Quaternion Quaternion::getInversa() {
    float sqr = w * w + x * x + y * y + z * z;
    return Quaternion(w / sqr, -x / sqr, -y / sqr, -z / sqr);
}

/**
* @brief Esta funcion calcula la Interpolacion Esferica Lineal
* (SLERP en ingles) del quaternion q2 al quaternion q1
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:22:39 AM
* @retval Quaternion Quaternion resultado de la interpolacion
* @param q1 Quaternion 1
* @param q2 Quaternion 2
* @param t va de 0.0 (en q1) hasta 1.0 (en q2)
*/
Quaternion Quaternion::SLERP(const Quaternion &q1, const Quaternion &q2, float t) {
    /**
    * qm = (q1* sin(theta * (1 - t)) + q2 * sin(t * theta))/sin(theta)
    *
    * donde:
    * - qm = quaternion resultante de la interpolacion
    * - q1 = quaternion 1
    * - q2 = quaternion 2
    * - t = va de 0.0 (en q1) hasta 1.0 (en q2)
    * - theta es la mitad del angulo entre ambos Quaterniones
    */

    // Si q1 == q2 entonces no hacemos los calculos y regresamos el mismo quaternion
    if (q1.w == q2.w && q1.x == q2.x && q1.y == q2.y && q1.z == q2.z) { return q1; }

    /*
    * cos(theta) = q1.q2 = (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z)
    * recordemos que theta es la mitad del angulo entre ambos Quaterniones
    **/
    float cos_theta = (q1.w * q2.w) + (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z);

    // checamos que el angulo sea mayor a 90; cos(90) = 0;
    if (cos_theta < 0.0F) {
        // negamos el segundo quaternion y el angulo
        (Quaternion) q2 = Quaternion(-q2.x, -q2.y, -q2.z, -q2.w);
        cos_theta = -cos_theta;
    }

    /*
    *Colocamos los valores iniciales de las escalas para cada termino
    * en caso de que el angulo sea despreciable
    * escala_q1 -> representa por lo que sera multiplicado q1
    * escala_q2 -> representa por lo que sera multiplicado q2
    **/
    float escala_q1 = 1 - t, escala_q2 = t;


    // Solamente si el angulo es muy grande hacemos los calculos
    if (1 - cos_theta > 0.1F) {

        // obtenemos el angulo y calculamos sin(theta)
        float theta = acos(cos_theta);
        float sin_theta = sin(theta);

        escala_q1 = (float) sin((1 - t) * theta) / sin_theta;
        escala_q2 = (float) (sin(t * theta)) / sin_theta;
    }


    // Finalmente multiplicamos escala_q1 * q sumandolo con escala_q2 * q2
    Quaternion resultado;
    resultado.x = (escala_q1 * q1.x) + (escala_q2 * q2.x);
    resultado.y = (escala_q1 * q1.y) + (escala_q2 * q2.y);
    resultado.z = (escala_q1 * q1.z) + (escala_q2 * q2.z);
    resultado.w = (escala_q1 * q1.w) + (escala_q2 * q2.w);

    return resultado;
}

/**
* @brief Regresa la magnitud de un Quaternion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:30:48 AM
* @retval float Regresa la magnitud de un Quaternion  
*/
inline float Quaternion::getMagnitud() {
    return sqrt(w * w + x * x + y * y + z * z);
}

/**
* @brief Nos dice si un Quaternion es unitario
* es decir si w^2 + x^2 + y^2 + z^2 = 1
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:31:32 AM
* @retval bool verdadero si es unitario, falso en caso contrario
*/
bool Quaternion::isUnitario() {
    if ((int) (w * w + x * x + y * y + z * z - 1.0F) == 0) {
        return true;
    }
    return false;
}

/**
* @brief Convierte el Quaternion actual a una matriz homogenea 
* de 4x4
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:32:52 AM
* @param m4x4 Arreglo donde se guardara la matriz de 4x4
*/
void Quaternion::to4x4Matriz(float *m4x4) {
    if (!m4x4) { return; }

    /*renglon 1**/
    m4x4[0] = 1.0F - 2.0F * (y * y + z * z);
    m4x4[1] = 2.0F * (x * y - w * z);
    m4x4[2] = 2.0F * (x * z + w * y);
    m4x4[3] = 0.0F;

    /*renglon 2**/
    m4x4[4] = 2.0F * (x * y + w * z);
    m4x4[5] = 1.0F - 2.0F * (x * x + z * z);
    m4x4[6] = 2.0F * (y * z - w * x);
    m4x4[7] = 0.0F;

    /*renglon 3**/
    m4x4[8] = 2.0F * (x * z - w * y);
    m4x4[9] = 2.0F * (y * z + w * x);
    m4x4[10] = 1.0F - 2.0F * (x * x + y * y);
    m4x4[11] = 0.0F;

    /*renglon 4**/
    m4x4[12] = 0;
    m4x4[13] = 0;
    m4x4[14] = 0;
    m4x4[15] = 1.0F;

}

/**
* @brief Convierte el Quaternion actual a una matriz 
* de 3x3
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:32:52 AM
* @param m3x3 Arreglo donde se guardara la matriz de 3x3
*/
void Quaternion::to3x3Matriz(float *m3x3) {
    if (!m3x3) { return; }

    /*renglon 1**/
    m3x3[0] = 1.0F - 2.0F * (y * y + z * z);
    m3x3[1] = 2.0F * (x * y - w * z);
    m3x3[2] = 2.0F * (x * z + w * y);

    /*renglon 2**/
    m3x3[3] = 2.0F * (x * y + w * z);
    m3x3[4] = 1.0F - 2.0F * (x * x + z * z);
    m3x3[5] = 2.0F * (y * z - w * x);

    /*renglon 3**/
    m3x3[6] = 2.0F * (x * z - w * y);
    m3x3[7] = 2.0F * (y * z + w * x);
    m3x3[8] = 1.0F - 2.0F * (x * x + y * y);

}

/**
* @brief Regresa un arreglo de chars que representa
* la informacion del Quaternion
* @author Fernando Montes de Oca Cspedes
* @date Monday, October 29, 2007 12:34:24 AM
* @retval char Arreglo de chars que representa
* la informacion del Quaternion
*/
char *Quaternion::toString() {
    char buff[32];
    sprintf(m_buff, "%.2f + %.2fi + %.2fj + %.2fk", w, x, y, z);
    return buff;
}