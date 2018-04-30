
/**
*@file Perlin.h
*@brief Contiene la clase de Perlin que sirve para hacer
*ruido de perlin
*/


#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <math.h>
#include <stdlib.h>
#include <time.h>


/**	@class Perlin
*	@author Fernando Montes de Oca Cspedes.
*	@version 1.0
*	@date	09/Jul/2007
*	@brief Tiene funciones necesarias para generar texturas
*	usando ruido de Perlin
*/
class Perlin {

public:

    /**
    *@brief Esta funcin es la que genera el ruido, generador
    *pseudo-aleatorio de nmeros y regresa el mismo nmero cuando
    *se le pasan los mismos factores.
    *@param x Es la posicin en x de la textura de ruido
    *@param y Es la posicin en y de la textura de ruido
    *@param random factor que sirve para asegurar que las nubes
    *son difernentes
    *@retval Regresa un valor de ruido que ser guardado en el mapa de ruido,
    *el valor que regresa es el mismo para mismos argumentos
    */
    static inline float noise(int x, int y, int random) {
        int n = x + y * 57 + random * 131;
        n = (n << 13) ^ n;
        return (1.0F - ((n * (n * n * 15731 + 789221) +
                         1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
    }


    /**
    *@brief Esta funcin es la que inicializa el mapa de 32x32 con rudio
    *@param map32 es un apuntador a un arreglo de 32x32
    *@todo Cambiar la funcion para que el mapa pueda ser de tamao variable
    */
    static void setNoise(float *map32) {
        float temp[34][34];
        srand((unsigned int) time(NULL));
        int random = rand() % 5000;

        for (int y = 1; y < 33; y++) {
            for (int x = 1; x < 33; x++) {
                temp[x][y] = 128.0F + noise(x, y, random) * 128.0F;
            }
        }

        for (int x = 1; x < 33; x++) {
            temp[0][x] = temp[32][x];
            temp[33][x] = temp[1][x];
            temp[x][0] = temp[x][32];
            temp[x][33] = temp[x][1];
        }
        temp[0][0] = temp[32][32];
        temp[33][33] = temp[1][1];
        temp[0][33] = temp[32][1];
        temp[33][0] = temp[1][32];


        for (int y = 1; y < 33; y++) {
            for (int x = 1; x < 33; x++) {
                float center = temp[x][y] / 4.0F;
                float sides = (temp[x + 1][y] + temp[x - 1][y] + temp[x][y + 1] + temp[x][y - 1]) / 8.0F;
                float corners =
                        (temp[x + 1][y + 1] + temp[x + 1][y - 1] + temp[x - 1][y + 1] + temp[x - 1][y - 1]) / 16.0F;

                map32[((x - 1) * 32) + (y - 1)] = center + sides + corners;
            }
        }
    }

    /**
    *@brief Esta funcin es la que se utiliza para interpolar el
    *ruido generado y es bsciamente la que hace el suavizado(smooth) del mismo
    *@param x Es la posicion en x del mapa de ruido multiplicado
    *por la escala que se quiere tener
    *@param y Es la posicion en y del mapa de ruido multiplicado
    *por la escala que se quiere tener
    *@param map Es un apuntador al mapa de rudio de 32x32
    *@retval regresa el valor de ruido interpolado de acuerdo con la escala deseada
    *@todo Cambiar la funcion para que el mapa pueda ser de tamao variable
    */
    static inline float interpolate(float x, float y, float *map) {
        int Xint = (int) x;
        int Yint = (int) y;

        float Xfrac = x - Xint;
        float Yfrac = y - Yint;

        int X0 = Xint % 32;
        int Y0 = Yint % 32;
        int X1 = (Xint + 1) % 32;
        int Y1 = (Yint + 1) % 32;

        float bot = map[X0 * 32 + Y0] + Xfrac * (map[X1 * 32 + Y0] - map[X0 * 32 + Y0]);
        float top = map[X0 * 32 + Y1] + Xfrac * (map[X1 * 32 + Y1] - map[X0 * 32 + Y1]);

        return (bot + Yfrac * (top - bot));
    }

    /**
    *@brief Esta funcin es la que inicializa el arreglo donde se va a guardar la textura en cero
    *ruido generado y es bsciamente la que hace el suavizado(smooth) del mismo
    *@param map Es un apuntador al mapa definitivo de la textura generada por rudio
    *@param width Es el ancho de la textura
    *@param height Es el alto de la textura
    */
    static inline void resetOctaves(float *map, int width, int height) {
        for (int x = 0; x < width * height; x++) {
            map[x] = 0;
        }
    }

    /**
    *@brief Esta funcin es la que hace el sobrepuesto (overlap) del octavo indicado
    *@param map32 Es un apuntador al mapa de rudio de 32x32
    *@param map Es un apuntador al mapa que tendr la textura definitiva
    *@param width Es el ancho de la textura deseada
    *@param height Es el alto de la textura deseada
    *@param octave Es el numero del octavo a ser sobrepuesto(overlapped)
    *@param scale Es la escala del octavo
    *@todo Cambiar la funcion para que el mapa de ruido pueda ser de tamao variable,
    *se tendrian que agregar las declaraciones de los parametros para el ancho y el alto
    *de la textura de ruido
    */
    static void overlapOctave(float *map32, float *map, int width, int height, int octave, float scale) {

        float noise = 0.0F;

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                noise = interpolate(x * scale, y * scale, map32);
                map[(y * height) + x] += noise / (1 << octave);

            }
        }

    }

    /**
    *@brief Esta funcin es la que hace el sobrepuesto (overlap) de todos los octavos
    *ruido generado y es bsciamente la que hace el suavizado(smooth) del mismo
    *@param map32 Es un apuntador al mapa de rudio de 32x32
    *@param map Es un apuntador al mapa que tendr la textura definitiva
    *@param width Es el ancho de la textura deseada
    *@param height Es el alto de la textura deseada
    *@todo Cambiar la funcion para que el mapa de ruido pueda ser de tamao variable,
    *se tendrian que agregar las declaraciones de los parametros para el ancho y el alto
    *de la textura de ruido
    */
    static void overlapOctaves(float *map32, float *map, int width, int height) {

        resetOctaves(map, width, height);

        float scale = 0.0F;
        float brightness = 0.0F;

        for (int octave = 0; octave < 4; octave++) {
            scale = 1 / pow(2, 3 - octave);

            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    brightness = interpolate(x * scale, y * scale, map32);
                    map[(y * height) + x] += brightness / (1 << octave);

                }
            }
        }
    }

    /**
    *@brief Esta funcin es la que hace el el filtro exponencial una vez que ya se hizo
    *el sobrepuesto (overlap) de los octavos, se le pasa el filtro para resaltar las
    *diferencias en el ruido generado
    *@param map Es un apuntador al mapa que tendr la textura definitiva
    *@param width Es el ancho de la textura deseada
    *@param height Es el alto de la textura deseada

    *@param cover Es el factor que indica que tan cubierto va a estar el cielo
    *@param sharpness Es el factor de delgadez de las nubes
    */
    static inline void expFilter(float *map, int width, int height, float cover, float sharpness) {

        for (int x = 0; x < width * height; x++) {
            float c = map[x] - (255.0F - cover);
            if (c < 0) c = 0;
            map[x] = 255.0F - ((float) (pow(sharpness, c)) * 255.0F);
        }
    }
};


#endif
