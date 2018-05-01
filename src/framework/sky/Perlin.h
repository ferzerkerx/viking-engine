#ifndef __PERLIN_H__
#define __PERLIN_H__

#include <cmath>
#include <cstdlib>
#include <ctime>

/**	@class Perlin
*	@author Fernando Montes de Oca Cespedes.
*	@version 1.0
*	@date	09/Jul/2007
*/
class Perlin {

public:

    static inline float Noise(int x, int y, int random) {
        int n = x + y * 57 + random * 131;
        n = (n << 13) ^ n;
        return (1.0F - ((n * (n * n * 15731 + 789221)
                         + 1376312589) & 0x7fffffff) * 0.000000000931322574615478515625f);
    }

    static void SetNoise(float *map32) {
        float temp[34][34];
        srand((unsigned int) time(nullptr));
        int random = rand() % 5000;

        for (int y = 1; y < 33; y++) {
            for (int x = 1; x < 33; x++) {
                temp[x][y] = 128.0F + Noise(x, y, random) * 128.0F;
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

    static inline float Interpolate(float x, float y, const float *map) {
        auto Xint = (int) x;
        auto Yint = (int) y;

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

    static inline void ResetOctaves(float *map, int width, int height) {
        for (int x = 0; x < width * height; x++) {
            map[x] = 0;
        }
    }

    static void OverlapOctave(float *map32, float *map, int width, int height, int octave, float scale) {
        float noise;

        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                noise = Interpolate(x * scale, y * scale, map32);
                map[(y * height) + x] += noise / (1 << octave);

            }
        }

    }

    static void OverlapOctaves(float *map32, float *map, int width, int height) {
        ResetOctaves(map, width, height);

        float scale;
        float brightness;

        for (int octave = 0; octave < 4; octave++) {
            scale = static_cast<float>(1 / pow(2, 3 - octave));

            for (int x = 0; x < width; x++) {
                for (int y = 0; y < height; y++) {
                    brightness = Interpolate(x * scale, y * scale, map32);
                    map[(y * height) + x] += brightness / (1 << octave);

                }
            }
        }
    }


    static inline void expFilter(float *map, int width, int height, float cover, float sharpness) {
        for (int x = 0; x < width * height; x++) {
            float c = map[x] - (255.0F - cover);
            if (c < 0) {
                c = 0;
            }
            map[x] = 255.0F - ((float) (std::pow(sharpness, c)) * 255.0F);
        }
    }
};


#endif
