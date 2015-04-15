/*
 * puntoOpcional.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al apartado opcional del trabajo. Consiste en calcular
 * 				a partir de una imagen el modulo y la orientacion
 * 				de su gradiente, con el fin de obtener un punto
 * 				de fuga bajo la suposicion de que la camara se
 * 				encuentra en una posicion ligeramente inclinada.
 * 				Para ello, se utiliza una franja como linea de
 * 				horizonte.
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <math.h>

using namespace std;
using namespace cv;

void puntoOpcional(){
	Mat imgT,imgDraw,imgPuntoFuga,gradH,gradV,gradM,gradO,gradMdraw,gradOdraw;
	int amplitudHorizonte = 150;

	Mat img = imread("Imagenes/pasillo1.pgm",CV_LOAD_IMAGE_GRAYSCALE);
	imshow("Imagen original",img);
	waitKey(0);
	cvtColor(img,imgDraw,CV_GRAY2BGR);
	imgPuntoFuga = imgDraw.clone();

	/* Aplica un filtro gaussiano a la imagen para reducir el ruido */
	GaussianBlur(img,imgT,Size(3,3),0,0,BORDER_DEFAULT);

	/* Si la imagen esta a color */
	imshow("Imagen tratada",imgT);
	waitKey(0);

	/* Componente horizontal del gradiente */
	Sobel(imgT, gradH, CV_32F, 1, 0, 3);

	/* Componente vertical del gradiente */
	Sobel(imgT, gradV, CV_32F, 0, 1, 3);

	/* Calcula el modulo del gradiente */
	magnitude(gradH,gradV,gradM);
	convertScaleAbs(gradM,gradMdraw);

	/* Calcula la orientacion del gradiente */
	phase(gradH,gradV,gradO);
	convertScaleAbs(gradO, gradOdraw,128 / M_PI);

	/* Muestra el modulo y la orientacion del gradiente de toda la imagen */
	imshow("Módulo del gradiente",gradMdraw);
	waitKey(0);
	imshow("Orientación del gradiente",gradOdraw);
	waitKey(0);

	/* Dibuja la linea del horizonte */
	rectangle(imgDraw,Point (-1,img.rows/2 - amplitudHorizonte/2),
					Point (img.cols,img.rows/2 + amplitudHorizonte/2),Scalar(255,0,0),1,8);
	imshow("Franja del horizonte",imgDraw);
	waitKey(0);

	/* Realiza la votacion de los puntos de contorno */
	double var = 0.1;
	double umbral = 100;

	Mat votacion = Mat::zeros(img.rows,img.cols,-1);

	for(int i = 0; i < img.rows; i++){
		for(int j = 0; j < img.cols; j++){

			/* Calcula la interseccion de cada punto de contorno con la linea de horizonte */
			double orientacion = gradO.at<float>(i,j);
			double modulo = gradM.at<float>(i,j);

			if (modulo >= umbral) {

				/* Obtiene el seno y coseno de la orientacion del gradiente */
				double sinO = sin(orientacion);
				double cosO = cos(orientacion);

				if (abs(sinO) > var && abs(cosO) > var) {

					/* Calcula los parametros ro y theta que definen a la recta */
					double ro = j * cosO + i * sinO;

					/* Calcula las distintas intersecciones con la franja */
					for(int k = 0; k < amplitudHorizonte; k++){
						int y = img.rows/2 - amplitudHorizonte/2 + k;
						int x = cvRound( ( ro - y * sinO) / cosO );

						if (x >= 0 && x < votacion.cols){

							/* Realiza la votacion al punto de interseccion de la franja */
							votacion.at<float>(y,x) = votacion.at<float>(y,x) + 1;
						}
					}
				}
			}
		}
	}

	/* Obtiene la componente con mayor numero de votos (punto de fuga) */
	int pfx = 0;
	int pfy = 0;

	for(int i = 0; i < votacion.rows; i++){
		for(int j = 0; j < votacion.cols; j++){
			if (votacion.at<float>(pfx,pfy) < votacion.at<float>(i,j)) {
				pfx = i;
				pfy = j;
			}
		}
	}

	int votos = votacion.at<float>(pfx,pfy);
	cout << "Punto de fuga: (" << pfx << ", " << pfy << ") -> " << votos << " votos recibidos." << endl;

	/* Dibuja el punto de fuga en la imagen */
	line(imgPuntoFuga,Point (pfy-10,pfx+10),Point (pfy+10,pfx-10),Scalar(0,0,255),1,8);
	line(imgPuntoFuga,Point (pfy-10,pfx-10),Point (pfy+10,pfx+10),Scalar(0,0,255),1,8);

	imshow("Punto de fuga",imgPuntoFuga);
	waitKey(0);
}
