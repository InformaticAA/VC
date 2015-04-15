/*
 * poster.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al efecto de posterizado o reduccion de color.
 *
 * NOTA: El codigo de este fichero esta basado en el encontrado
 * 		 en las diapositivas de clase del tema 1.
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void reducirColor(cv::Mat &img, int div);

void reducirColor(cv::Mat &img, int div) {
	for (int i=0; i<img.rows; i++) {
		uchar* data= img.ptr<uchar>(i);  // puntero a la fila i
		for (int j=0; j<img.cols*img.channels(); j++) {
			data[j]= data[j]/div*div + div/2;
		}
	}
}
