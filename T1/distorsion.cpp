/*
 * distorsion.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al efecto de distorsion, tanto de cojin como de
 * 				barril.
 *
 * NOTA: El codigo de este fichero esta basado en un ejemplo encontrado en
 * 		 la documentacion de opencv. Se han realizado mofificaciones para
 * 		 incluir los efectos de distorsion ajustables. El enlace a dicha
 * 		 pagina se proporciona a continuacion:
 *
 * URL: http://docs.opencv.org/doc/tutorials/imgproc/imgtrans/remap/remap.html
 *
 */
#include <opencv2\opencv.hpp>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace cv;
using namespace std;

Mat distorsion(cv::Mat &img, int tipo, double modificador);

Mat distorsion(cv::Mat &img, int tipo, double modificador){

	Mat distorsion;
	Mat mapx;
	Mat mapy;

	mapx.create(img.size(), CV_32FC1 );
	mapy.create(img.size(), CV_32FC1 );

	int Cx = img.rows/2;
	int Cy = img.cols/2;

	float x;
	float y;
	float r2;
	double k1;
	double k2;

	/* Elige entre los dos tipos de distorsion disponibles */
	if (tipo == 1) { //Distorsion de cojin
		k1 = 0.00000000001 + modificador;
		k2 = 0.00000000001 + modificador;
	} else { //Distorsion de barril
		k1 = - 0.00000000001 + modificador;
		k2 = - 0.00000000001 + modificador;
	}

	/* Recorre la matriz obteniendo las nuevas coordenadas de cada pixel */
	for(int i = 0; i < img.rows; i++){
		for(int j = 0; j < img.cols; j++){
			r2 = (i-Cx)*(i-Cx) + (j-Cy)*(j-Cy);
			x = (i-Cx)/(1 + k1*r2 + k2*r2*r2) + Cx;
			y = (j-Cy)/(1 + k1*r2 + k2*r2*r2) + Cy;
			mapx.at<float>(i,j) = saturate_cast<double>(x);
			mapy.at<float>(i,j) = saturate_cast<double>(y);
		}
	}
	remap( img, distorsion, mapy, mapx, CV_INTER_LINEAR);
	return distorsion;
}
