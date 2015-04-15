/*
 * umbralizar.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al proceso de umbralizado de una imagen. Por
 * 				otro lado, se aplica una operacion de morfología
 * 				una vez obtenida la imagen binaria, con el fin
 * 				de limpiar de ruido de sal el resultado.
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat umbralizar(cv::Mat &img);

Mat umbralizar(cv::Mat &img) {
	threshold(img,img,0,255,CV_THRESH_BINARY_INV | CV_THRESH_OTSU);
	return img;
}
