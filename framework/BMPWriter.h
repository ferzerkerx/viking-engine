/**
* @file BMPWriter.h
* @brief Encabezado de la clase BMPWriter
* @date Saturday, October 20, 2007 5:50:37 PM
*/
#ifndef __BMPWRITER_H__
#define __BMPWRITER_H__

#include "TextureWriter.h"

/**
* @class BMPWriter
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 5:50:41 PM
* @brief Clase qeu escribe BMP's de 24 bits 
*/ 
class BMPWriter : public TextureWriter{
	private:
		void escribeFileHeader();
		void escribeInfoHeader();
		void escribeData();

		struct BITMAPFILEHEADER{			//start 	size 	name 		stdvalue 	purpose
			short			bfType;			//  1 	     2 	   bfType 	     19778 		must always be set to 'BM' to declare that this is a .bmp-file.
			unsigned long	bfSize;			//  3 	     4 	   bfSize 	      ?? 		specifies the size of the file in bytes.
			short    	    bfReserved1;	//  7  	     2 	   bfReserved1 	   0 		must always be set to zero.
			short           bfReserved2;	//  9 	     2 	   bfReserved2 	   0 		must always be set to zero.
			unsigned int    bfOffBits;		//  11 	     4 	   bfOffBits 	  54 		specifies the offset from the beginning of the file to the bitmap data.
		}m_file_header; //representa un BITMAPFILEHEADER


		struct BITMAPINFOHEADER{						//start 	size 	name 			stdvalue 	purpose
				unsigned long int   biSize;				// 15 		4 		biSize 				40 		specifies the size of the BITMAPINFOHEADER structure, in bytes.
				long       			biWidth;			// 19 		4 		biWidth 			100 	specifies the width of the image, in pixels.
				long       			biHeight;			// 23 		4 		biHeight 			100 	specifies the height of the image, in pixels.	
				short     			biPlanes;			// 27 		2 		biPlanes 			1 		specifies the number of planes of the target device, must be set to zero.
				short       		biBitCount;			// 29 		2 		biBitCount 			8	 	specifies the number of bits per pixel.
				int      			biCompression;		// 31 		4 		biCompression 		0 		Specifies the type of compression, usually set to zero (no compression).
				int      			biSizeImage;		// 35 		4 		biSizeImage 		0 		specifies the size of the image data, in bytes. If there is no compression, it is valid to set this member to zero.
				long     			biXPelsPerMeter;	// 39 		4 		biXPelsPerMeter 	0 		specifies the the horizontal pixels per meter on the designated targer device, usually set to zero.
				long       			biYPelsPerMeter;	// 43 		4 		biYPelsPerMeter 	0 		specifies the the vertical pixels per meter on the designated targer device, usually set to zero.
				int      			biClrUsed;			// 47 		4 		biClrUsed 			0 		specifies the number of colors used in the bitmap, if set to zero the number of colors is calculated using the biBitCount member.
				int      			biClrImportant;		// 51 		4 		biClrImportant 		0 		specifies the number of color that are 'important' for the bitmap, if set to zero, all colors are important.
		}m_info_header;	//representa un BITMAPINFOHEADER

		const char * m_data; //apuntador a los datos de la imagen(colores)
		int m_prof;		//representa el numero de colores 3 = RGB, 4 = RGBA

	public:
		void writeTexture();
		BMPWriter(const char *file, int width, int height, int prof,char * data);
		~BMPWriter();
		
};


/**
* @var BMPWriter::m_file_header
* @brief representa un BITMAPFILEHEADER
*/ 

/**
* @var BMPWriter::m_data
* @brief apuntador a los datos de la imagen(colores)
*/

/**
* @var BMPWriter::m_prof
* @brief representa el numero de colores 3 = RGB, 4 = RGBA
*/


#endif