/*
 * calcularDescriptores.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				a las operaciones de calculo de descriptores
 * 				para una imagen binaria, estos son:
 * 					-Area
 * 					-Perimetro
 * 					-Momento invariante 1
 * 					-Momento invariante 2
 * 					-Momento invariante 3
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void obtenerArea(vector <vector <Point> > &contornos,vector <double> &areas);
void obtenerPerimetro(vector <vector <Point> > &contornos,vector <double> &perimetros);
void obtenerMomentos(vector <vector <Point> > &contornos,vector <double> &hu_mom_1,vector <double> &hu_mom_2, vector <double> &hu_mom_3);

void obtenerArea(vector <vector <Point> > &contornos,vector <double> &areas){
	double area;
	for(int i = 0; i < contornos.size(); i++){
		area = contourArea(contornos[i], false);
		areas.push_back(area);
	}
}

void obtenerPerimetro(vector <vector <Point> > &contornos,vector <double> &perimetros){
	double perimetro;
	for(int i = 0; i < contornos.size(); i++){
		perimetro = arcLength(contornos[i],true);
		perimetros.push_back(perimetro);
	}
}

void obtenerMomentos(vector <vector <Point> > &contornos,vector <double> &hu_mom_1,vector <double> &hu_mom_2, vector <double> &hu_mom_3){
	Moments mom;
	double hu_moments[7];
	for(int i = 0; i < contornos.size(); i++){
		mom = moments(contornos[i],false);
		HuMoments(mom, hu_moments);
		hu_mom_1.push_back(hu_moments[0]);
		hu_mom_2.push_back(hu_moments[1]);
		hu_mom_3.push_back(hu_moments[2]);
	}
}
