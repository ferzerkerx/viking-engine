/***********************************************************************
 *
 * @autor: Luis Daniel Rodrguez Palazuelos
 * 
 * @date: Julio 2006
 *
 * Carga texturas para Opengl, BMP, Jpg y Tga
 *
 *
 ************************************************************************/

#include <cstring>
#include "Textura.h"
#include "GL/glut.h"


Textura::Textura() {
	hasAlpha = false;
}

void Textura::CrearTextura(unsigned int textureArray[], char * strFileName, int textureID) {
	if(!strFileName)		// Sale si no es un nombre de archivo vlido
		return;

	char* pdest = strrchr(strFileName, '.' );		// encuentra '.'
	if(pdest[0]!= NULL)		// Avanzamos uno mas que '.'
		pdest++;

	if(!strcmp(pdest,"bmp")) {
		CreaBMP(textureArray, strFileName, textureID);
		hasAlpha = false;
		printf("Creando textura: %s \n", strFileName);
	}
	else if(!strcmp(pdest,"tga")) {
        CreaTGA(textureArray, strFileName, textureID);
		hasAlpha = true;
		printf("Creando textura: %s \n", strFileName);
	}
}

void Textura::CreaBMP(unsigned int textureArray[], char * strFileName, int textureID) {
    int width, height;

    unsigned char * data;

    FILE * file;

    file = fopen( strFileName, "rb" );

    if ( file == NULL ) {
        printf("Could not open file: %s \n", strFileName);
        return;
    }
    width = 256;
    height = 256;
    data = (unsigned char *)malloc( width * height * 3 );
    fread( data, width * height * 3, 1, file );
    fclose( file );

    for(int i = 0; i < width * height ; ++i) {
        int index = i*3;
        unsigned char B,R;
        B = data[index];
        R = data[index+2];

        data[index] = R;
        data[index+2] = B;

    }

    // Generamos texturas segun OpenGL
	glGenTextures(1, &textureArray[textureID]);
	//glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);   
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	
	// Liberamos los datos de la imagen
	if(data) {
	    free(data);
	}
}

void Textura::CreaTGA(unsigned int textureArray[], char * strFileName, int textureID) {
    
	// Apuntador a datos de la imagen
	tImageTGA *pImage = CargaTGA(strFileName);	// Carga imagen y guarda los datos

	if(pImage == NULL)		// Checamos que se cargue la imagen
		return

	// Generate la textura conforme a OpenGL
	glGenTextures(1, &textureArray[textureID]);
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	int textureType = GL_RGB;

	if(pImage->channels == 4)		// Si la imagen es de 32bits decimos que tiene alpha
		textureType = GL_RGBA;
		
	gluBuild2DMipmaps(GL_TEXTURE_2D, pImage->channels, pImage->sizeX, 
					  pImage->sizeY, textureType, GL_UNSIGNED_BYTE, pImage->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	

	// Liberamos la imagen
	if(pImage) {
		if(pImage->data) {
			delete[] pImage->data;
		}
		free(pImage);	
	}
}

// Carga la imagen TGA y regresa sus datos
tImageTGA *Textura::CargaTGA(const char *filename) {

	tImageTGA *pImageData = NULL;		// Datos de la imagen
	unsigned short width = 0, height = 0;			// Dimensiones de la imagen
    unsigned char length = 0;					// Longitud en bytes de pixeles
    unsigned char imageType = 0;					// Tipo de imagen (RLE, RGB, Alpha...)
    unsigned char bits = 0;						// The bits per pixel for the image (16, 24, 32)
	FILE *pFile = NULL;					
	int channels = 0;					// Canales de la imagen (3 = RGA : 4 = RGBA)
	int stride = 0;						// (channels * width)
	int i = 0;							
		
	if((pFile = fopen(filename, "rb")) == NULL) {	// Abrimos archivo
		printf("TGA: %s \n", filename);
		return NULL;
	}
		
	pImageData = (tImageTGA*)malloc(sizeof(tImageTGA));	// Guardamos espacio a la estructura

	fread(&length, sizeof(unsigned char), 1, pFile);	// Leemos el header
	fseek(pFile,1,SEEK_CUR); 
	fread(&imageType, sizeof(unsigned char), 1, pFile);	// Leemos tipo de imagen (RLE, RGB, Alpha)
	fseek(pFile, 9, SEEK_CUR);	// Nos saltamos info irrelevante 

	// Leemos ancho, alto y bits por pixel (16, 24 o 32)
	fread(&width,  sizeof(unsigned short), 1, pFile);
	fread(&height, sizeof(unsigned short), 1, pFile);
	fread(&bits,   sizeof(unsigned char), 1, pFile);
	
	fseek(pFile, length + 1, SEEK_CUR);	// Nos movemos a los datos de pixel 

	if(imageType != TGA_RLE) {	// Checamos si la imagen tiene compresin
		if(bits == 24 || bits == 32) {	// Checamos si es de 24 o 32 bits
			channels = bits / 8;	// Calculamos canal (3-4)
			stride = channels * width;	// Calculamos stride y le hacemos espacio en memoria
			pImageData->data = new unsigned char[stride * height];

			for(int y = 0; y < height; y++) {	// Cargamos pixeles linea por linea
				// Apuntador a la linea actual de pixeles
				unsigned char *pLine = &(pImageData->data[stride * y]);
				fread(pLine, stride, 1, pFile);	// Leemos la linea actual
			
				// Cambiamos B y R porque TGA se guardan como BGR en lugar de RGB
				for(i = 0; i < stride; i += channels) {
					int temp     = pLine[i];
					pLine[i]     = pLine[i + 2];
					pLine[i + 2] = temp;
				}
			}
		}
		else if(bits == 16) {	// Checamos si la imagen es de 16 bits
			unsigned short pixels = 0;
			int r=0, g=0, b=0;

			// Convertimos de 16-bit a 24 bit
			channels = 3;
			stride = channels * width;
			pImageData->data = new unsigned char[stride * height];

			for(int i = 0; i < width*height; i++) {
				fread(&pixels, sizeof(unsigned short), 1, pFile);
				
				// Para convertir a 16-bit a R, G, B, necesitamos aislar cada color, como
				// 0x1f = 11111 en binario, 5 bits estan reservados en cada unsigned short
				// para R, G y B. Cambiamos los valores por 3 para obtener el color final.
				b = (pixels & 0x1f) << 3;
				g = ((pixels >> 5) & 0x1f) << 3;
				r = ((pixels >> 10) & 0x1f) << 3;
				
				// Asignamos color al arreglo y cambiamos B por R al mismo tiempo.
				pImageData->data[i * 3 + 0] = r;
				pImageData->data[i * 3 + 1] = g;
				pImageData->data[i * 3 + 2] = b;
			}
		}
		else
			return NULL;
	}
	else {	// Si la imagen tiene compresion
        // RLE es tipo bsico de compresion, ejemplo: "aaaaabbcccccccc" se hace "a5b2c8"
		// Leemos un contador de color (rleID), si es menor a 128, no hay optimizacion asi
		// que solo leemos los siguientes pixeles. Si es mayor a 128, el siguiente color 
		// esta optimizado necesitamos leer el mismo pixel para el color (colorCount - 127).
		
		unsigned char rleID = 0;
		int colorsRead = 0;
		channels = bits / 8;
		stride = channels * width;

		pImageData->data = new unsigned char[stride * height];
		unsigned char *pColors = new unsigned char [channels];

		while(i < width*height) {
			fread(&rleID, sizeof(unsigned char), 1, pFile);	// Leemos color actual
			
			if(rleID < 128) {	// Checamos colores
				rleID++;
				while(rleID) {
					fread(pColors, sizeof(unsigned char) * channels, 1, pFile);

					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}
			}
			else {	// Checamos colores optimizados
				rleID -= 127;

				fread(pColors, sizeof(unsigned char) * channels, 1, pFile);

				while(rleID) {
					pImageData->data[colorsRead + 0] = pColors[2];
					pImageData->data[colorsRead + 1] = pColors[1];
					pImageData->data[colorsRead + 2] = pColors[0];

					if(bits == 32)
						pImageData->data[colorsRead + 3] = pColors[3];

					i++;
					rleID--;
					colorsRead += channels;
				}				
			}				
		}
		
		delete[] pColors;	// Liberamos colores
	}

	fclose(pFile);	// Cerramos el archivo

	// Llenamos nuestra estructura tImageTGA 
	pImageData->channels = channels;
	pImageData->sizeX    = width;
	pImageData->sizeY    = height;

	return pImageData;
}