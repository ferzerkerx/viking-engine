/**
* @file CloudsActualizables.cpp
* @brief Implementacin de la clase CloudsActualizables
* @date Friday, October 19, 2007 5:18:01 PM
*/

#include <GL/glut.h>
#include <stdio.h>
#include "CloudsActualizables.h"

/**
*@copydoc Clouds::Clouds()
*@brief Inicializa el tiempo de actualizacion
*/
CloudsActualizables::CloudsActualizables() : Clouds() {
    m_update_text_milli = 500;
    m_brightness = 0.0f;
    m_actual_octave = 0;

    m_octaves = new struct octave[m_num_octaves];
    for (int i = 0; i < m_num_octaves; i++) {
        m_octaves[i].scale = 1 / pow(2, 3 - i);
        m_octaves[i].x = 0;
    }

    m_map = new float[m_text_width * m_text_height];
    m_RGBA_text = new char[m_text_width * m_text_height * 4];

    Perlin::setNoise(m_map32);
    Perlin::overlapOctaves(m_map32, m_map, m_text_width, m_text_height);
    Perlin::expFilter(m_map, m_text_width, m_text_height, m_cover, m_sharpness);
    generaTextura();

}

/**
*@copydoc Clouds::Clouds(float cover, float sharpness, int num_octavos, int width, int height)
*@param up_text_milli define cada cuando se debe actualizar la textura
*/
CloudsActualizables::CloudsActualizables(float cover, float sharpness, int num_octavos, int width, int height,
                                         unsigned int up_text_milli)
        : Clouds(cover, sharpness, num_octavos, width, height) {
    m_update_text_milli = up_text_milli;
    m_brightness = 0.0f;
    m_actual_octave = 0;

    m_octaves = new struct octave[m_num_octaves];
    for (int i = 0; i < m_num_octaves; i++) {
        m_octaves[i].scale = 1 / pow(2, 3 - i);
        m_octaves[i].x = 0;
    }

    m_map = new float[m_text_width * m_text_height];
    m_RGBA_text = new char[m_text_width * m_text_height * 4];

    Perlin::setNoise(m_map32);
    Perlin::overlapOctaves(m_map32, m_map, m_text_width, m_text_height);
    Perlin::expFilter(m_map, m_text_width, m_text_height, m_cover, m_sharpness);
    generaTextura();
}

/**
 * @brief destructor de la clase libera los octavos usados
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:01:23 PM
 */
CloudsActualizables::~CloudsActualizables() {
    if (m_octaves != NULL) { delete[] m_octaves; }
}

/**
 * @brief Fuerza una actualizacin completa de la textura
 * @warning Bastante lenta puede generar un brinco mientras se juega
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:01:23 PM
 */
void CloudsActualizables::forceUpdate() {
    Perlin::overlapOctaves(m_map32, m_map, m_text_width, m_text_height);
    Perlin::expFilter(m_map, m_text_width, m_text_height, m_cover, m_sharpness);
    m_actual_octave = 0;
    m_text_timer.reset();
}

/**
*@brief Cambia el tiempo de actualizacin de la textura
*@param milli Es el tiempo deseado en milisegundos
*/
void CloudsActualizables::setTextureUpdateTime(int milli) {
    m_update_text_milli = milli;
}

/**
*@brief Cambia el nmero de octavos y con ello la estructura
*@param noctaves
*/
void CloudsActualizables::setNumOctaves(unsigned char noctaves) {
    m_num_octaves = noctaves;
    if (m_octaves != NULL) { delete[] m_octaves; }

    m_octaves = new struct octave[m_num_octaves];
    for (int i = 0; i < m_num_octaves; i++) {
        m_octaves[i].scale = 1 / pow(2, 3 - i);
        m_octaves[i].x = 0;
    }
}

/**
 * @brief Cambia el factor de nubosidad de la textura
 * @param cover Es la cantidad de nubes (nubosidad)
 */
void CloudsActualizables::setCover(float cover) {
    m_cover = cover;
}


/**
 * @brief Cambia el factor de delgadez de las nubes
 * @author Fernando Montes de Oca Cespedes 
 * @date 08-11-2007
 * @param sharpness factor de degadez deseado
 */
void CloudsActualizables::setSharpness(float sharpness) {
    m_sharpness = sharpness;
}


/**
 * @brief Cambia el tamao de la textura deseada
 * si ya haba una se borra y se redefine, se tendria que llamar
 * actualizar despues de esta llamada
 * @author Fernando Montes de Oca Cespedes 
 * @date 08-11-2007
 * @param width El ancho de la textura que se desea
 * @param height El alto de la textura que se desea
 */
void CloudsActualizables::setTextureResolution(int width, int height) {
    if (width < 32 || height < 32) return;
    if (m_RGBA_text != NULL) { delete[] m_RGBA_text; }
    if (m_map != NULL) { delete[] m_map; }

    m_text_width = width;
    m_text_height = height;

    m_octaves[m_actual_octave].x = 0;
    m_map = new float[m_text_width * m_text_height * 4];
    m_RGBA_text = new char[m_text_width * m_text_height * 4];
    m_actual_octave = 0;
    Perlin::resetOctaves(m_map, m_text_width, m_text_height);
}

/**
 * @brief Hace sobrepuesto de octavo(overlap) uno a la vez, buen desempeo
 * aunque no el ms eficiente
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:02:37 PM
 * @retval Regresa 0 si no se ha generado la textura
 *		   Regresa 1 si no se genero una textura nueva
 */
int CloudsActualizables::actualizaPorOctavo() {

    int act_millis = m_text_timer.getMilliseconds();

    /**actualizamos un octavo a la vez si es que se cumple el tiempo***/
    if ((m_actual_octave < m_num_octaves) &&
        (act_millis >= (m_update_text_milli / (m_num_octaves + 1)) * m_actual_octave)) {
        Perlin::overlapOctave(m_map32, m_map, m_text_width, m_text_height, m_actual_octave,
                              m_octaves[m_actual_octave].scale);
        m_actual_octave++;
    }

    /**genera la textura una vez que todos los octavos han sido sobrepuestos y si el tiempo de actualizacin se ha cumplido**/
    if ((m_actual_octave == m_num_octaves) && (m_text_timer.getMilliseconds() >= m_update_text_milli)) {
        m_text_timer.reset();
        Perlin::expFilter(m_map, m_text_width, m_text_height, m_cover, m_sharpness);
        m_actual_octave = 0;
        generaTextura();
        return 1;
    }

    return 0;
}

/**
 * @brief Actualiza por coordenada el octavo actual,
 * incrementa en 1 la x y hace todo el ciclo(y) con esa x
 * bastante eficiente
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 10:05:21 PM
 * @retval Regresa 0 si no se ha generado la textura
 *		   Regresa 1 si no se genero una textura nueva
  */
int CloudsActualizables::actualizaPorCoordenada() {
    int act_millis = m_text_timer.getMilliseconds();

    /***sumo al mapa actual lo apuntado por la linea x que representa m_actual_octave****/
    if ((m_actual_octave < m_num_octaves) &&
        (act_millis >= (m_update_text_milli / (m_num_octaves + 1)) * m_actual_octave)) {

        for (int y = 0; y < m_text_height; y++) {
            m_brightness = Perlin::interpolate(m_octaves[m_actual_octave].x * m_octaves[m_actual_octave].scale,
                                               y * m_octaves[m_actual_octave].scale, m_map32);
            m_map[(y * m_text_height) + m_octaves[m_actual_octave].x] += m_brightness / (1 << m_actual_octave);

        }

        if (m_octaves[m_actual_octave].x < m_text_width) {
            m_octaves[m_actual_octave].x++;
        } else {
            m_octaves[m_actual_octave].x = 0;
            m_actual_octave++;
        }
    }

    /**una vez que se terminaron de actualizar los octavos y que el tiempo
    se ha cumplido entonces se regenera la textura**/
    if ((m_actual_octave == m_num_octaves) && m_text_timer.getMilliseconds() >= m_update_text_milli) {
        m_text_timer.reset();
        Perlin::expFilter(m_map, m_text_width, m_text_height, m_cover, m_sharpness);
        m_actual_octave = 0;
        generaTextura();
        return 1;
    }

    return 0;
}

/**
 * @brief Genera la textura con el alto y el ancho especificado
 * agrega canal alpha al mapa de textura y destruye la textura anterior
 * @author Fernando Montes de Oca Cespedes 
 * @date Saturday, August 11, 2007 9:40:02 PM
 */
void CloudsActualizables::generaTextura() {

    Clouds::generaTexturaGL();
    Perlin::resetOctaves(m_map, m_text_width, m_text_height);
    //Perlin::setNoise(m_map32);
}
