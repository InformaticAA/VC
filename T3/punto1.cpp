/*
 * punto1.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al apartado 1 del trabajo. Consiste en calcular
 * 				a partir de una imagen, las componentes
 * 				horizontal y vertical del gradiente, el modulo
 * 				y la orientacion.
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;
using namespace cv;

Mat orientacionGradiente(Mat gradH,Mat gradV);

void punto1(){
	Mat gradH, gradV, gradHdraw, gradVdraw,gradM,gradO,contornos;

	Mat img = imread("Imagenes/poster.pgm",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Imagen original",img);
	waitKey(0);

	/* Aplica un filtro gaussiano a la imagen para reducir el ruido */
	GaussianBlur(img,img,Size(3,3),0,0,BORDER_DEFAULT);

	/* Si la imagen esta a color */
	imshow("Imagen tratada",img);
	waitKey(0);

	/* Componente horizontal del gradiente */
	Sobel(img, gradH, CV_32F, 1, 0, 3);

	/* Componente vertical del gradiente */
	Sobel(img, gradV, CV_32F, 0, 1, 3,-1);

	/* Calcula el modulo del gradiente */
	magnitude(gradH,gradV,gradM);
	convertScaleAbs(gradM,gradM); // Para dibujar

	/* Calcula la orientacion del gradiente */
	gradO = orientacionGradiente(gradH,gradV);

	/* Reescala los gradientes para poder mostrarlos */
	convertScaleAbs(gradH, gradHdraw, 0.5, 128);
	convertScaleAbs(gradV, gradVdraw, 0.5, 128);

	imshow("Gradiente horizontal",gradHdraw);
	waitKey(0);
	imshow("Gradiente vertical",gradVdraw);
	waitKey(0);
	imshow("Módulo del gradiente",gradM);
	waitKey(0);
	imshow("Orientación del gradiente",gradO);
	waitKey(0);
}

Mat orientacionGradiente(Mat gradH,Mat gradV){
	Mat gradO;
	double alpha = 128 / M_PI;

	phase(gradH,gradV,gradO);

	convertScaleAbs(gradO, gradO,alpha);

	return gradO;
}
