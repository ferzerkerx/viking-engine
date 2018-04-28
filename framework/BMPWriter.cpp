/**
* @file BMPWriter.cpp
* @brief Implementacin de la clase BMPWriter
* @date Saturday, October 20, 2007 5:41:44 PM
*/


#include "BMPWriter.h"

/**
* @copydoc TextureWriter::TextureWriter(const char *file)
* @brief Escribe la textura con el nombre especificado
* @param width Ancho de la textura
* @param height Alto de la textura
* @param prof num_colores (3 = RGB, 4 = RGBA)
* @param data apuntador a los datos de la imagen (debe ser de width x height x prof_color)
* @warning A pesar de que recibe datos con alpha, los ignora, recordemos que escribe bitmaps de 24 bits
*/
BMPWriter::BMPWriter(const char *file, int width, int height, int prof, char * data ) : TextureWriter(file){
	if (prof < 3){prof = 3;}
	
	m_file_header.bfType = 19778;
	m_file_header.bfSize = 54+(width*height*3);//profundidad 24 bits
	m_file_header.bfReserved1 = 0;
	m_file_header.bfReserved2 = 0;
	m_file_header.bfOffBits = 54;

	m_info_header.biSize = 40;
	m_info_header.biWidth = width;
	m_info_header.biHeight = height;
	m_info_header.biPlanes = 0;
	m_info_header.biBitCount = 24;//bits por pixel
	m_info_header.biHeight = height;
	m_info_header.biCompression = 0;
	m_info_header.biSizeImage = width*height*3;
	m_info_header.biXPelsPerMeter = 0;
	m_info_header.biYPelsPerMeter = 0;
	m_info_header.biClrUsed = 0;
	m_info_header.biClrImportant = 0;

	m_data = data;
	m_prof = prof;
}

/**
* @brief Simplemente coloca m_data en NULL
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 8:51:56 PM
*/
BMPWriter::~BMPWriter(){
	m_data = NULL;
}

/**
* @brief Escribe los encabezados y los datos de la textura
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 8:51:56 PM
*/
void BMPWriter::writeTexture(){
	escribeFileHeader();
	escribeInfoHeader();
	escribeData();
}

/**
* @brief escribe un BITMAPFILEHEADER
* NOTA: por alguna razon no puedo escribir la estructura directamente (linea comentada)
* porque escribe 2 bytes de mas
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 8:50:50 PM
*/
void BMPWriter::escribeFileHeader(){
	//m_out.write((char *) &m_file_header, sizeof(struct BITMAPFILEHEADER));	
	m_out.write((char *) &m_file_header.bfType, 2);
	m_out.write((char *) &m_file_header.bfSize, 4);
	m_out.write((char *) &m_file_header.bfReserved1, 2);
	m_out.write((char *) &m_file_header.bfReserved2, 2);
	m_out.write((char *) &m_file_header.bfOffBits, 4);
		
}


/**
* @brief Escribe el BITMAPINFOHEADER del BMP
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 8:50:21 PM
*/
void BMPWriter::escribeInfoHeader(){
	m_out.write((char *) &m_info_header, sizeof(struct BITMAPINFOHEADER));	
}

/**
* @brief Escribe la imagen en el archivo especificado
* si los datos no existen escribe una imagen con r=g=b= 0
* @author Fernando Montes de Oca Cspedes
* @date Saturday, October 20, 2007 8:49:21 PM
*/
void BMPWriter::escribeData(){
	
	int desplazamiento = 0 ;
	int i =0, j = 0;	
	int r = 0, g = 0, b = 0;
	
	for(i=0; i<m_info_header.biWidth; i++)   {     
		for(j=0; j<m_info_header.biHeight; j++) {

			if (m_data != NULL){
				desplazamiento = i*(m_info_header.biHeight*m_prof)+(j*m_prof);
				m_out.write(&m_data[desplazamiento+1],1);//b
				m_out.write(&m_data[desplazamiento], 1);//g
				m_out.write(&m_data[desplazamiento+2],1);//r
			}
			else{
				m_out.write((char *)&b,1);//b
				m_out.write((char *)&g, 1);//g
				m_out.write((char *)&r,1);//r
			}
		}		
	}	
}