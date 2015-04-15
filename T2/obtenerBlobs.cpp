/*
 * obtenerBlobs.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al proceso de obtencion de los contornos.
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;
void obtenerBlobs(cv::Mat &img,vector <vector <Point> > &contornos,vector <Vec4i> &jerarquia);
vector <vector <Point> > filtrar(vector <vector <Point> > &contornos, int umbral);
extern void obtenerArea(vector <vector <Point> > &contornos,vector <double> &areas);

void obtenerBlobs(cv::Mat &img,vector <vector <Point> > &contornos,vector <Vec4i> &jerarquia){
	findContours(img,contornos, jerarquia, CV_RETR_CCOMP ,CV_CHAIN_APPROX_NONE);

	/* Realiza un filtro de los contornos por numero de pixels */
	contornos = filtrar(contornos, 300);
}

vector <vector <Point> > filtrar(vector <vector <Point> > &contornos, int umbral){
	vector <vector <Point> > contornosF;
	vector <double> areas;
	obtenerArea(contornos,areas);
	for(int i = 0; i < contornos.size(); i++){
		if(areas[i]>=umbral){
			contornosF.push_back(contornos[i]);
		}
	}
	return contornosF;
}
