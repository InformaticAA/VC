/*
 * contraste.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				a la mejora de contraste. Se pueden aplicar dos
 *				tipos de modificacion:
 *
 *						-Cambio manual
 *						-Ecualizacion de histograma
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void cambiarContrasteManual(cv::Mat &img, double modificacion);
Mat ecualizacionHistograma(cv::Mat &img);

void cambiarContrasteManual(cv::Mat &img, double modificacion) {
	img.convertTo(img,-1,modificacion,0);
}

Mat ecualizacionHistograma(cv::Mat &img) {
	Mat result;

	/* Realiza la ecualizacion de histograma */
	if(img.channels() >= 3) {
		Mat ycrcb;

		/* Pasa a un espacio de color con la luminosidad por separado */
		cvtColor(img,ycrcb,CV_BGR2YCrCb);

		/* Separa los canales de la imagen original */
		vector<Mat> channels;
		split(ycrcb,channels);

		/* Equaliza el histograma del canal 0 (luminosidad) */
		equalizeHist(channels[0],channels[0]);

		/* Incluye de nuevo el canal modificado (luminosidad) */
		merge(channels,ycrcb);

		/* Vuelve al espacio de color original */
		cvtColor(ycrcb,result,CV_YCrCb2BGR);
	}
	return result;
}
