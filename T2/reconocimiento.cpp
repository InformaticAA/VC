/*
 * reconocimiento.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al algoritmo de reconocimiento. Se encarga de
 * 				reconocer los objetos de una imagen binaria a
 * 				partir de sus descriptores y asociar el resultado
 * 				con determiandas clases preaprendidas.
 *
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace cv;

extern Mat umbralizar(cv::Mat &img);
extern void obtenerBlobs(cv::Mat &img,vector <vector <Point> > &contornos,vector<Vec4i> &jerarquia);
extern void obtenerArea(vector <vector <Point> > &contornos,vector <double> &areas);
extern void obtenerPerimetro(vector <vector <Point> > &contornos,vector <double> &perimetros);
extern void obtenerMomentos(vector <vector <Point> > &contornos,vector <double> &hu_mom_1,vector <double> &hu_mom_2, vector <double> &hu_mom_3);
vector <vector <String> > reconocer(String nomFich);
double distanciaMahalanobis(vector<double> muestra, vector<double> medias, vector<double> varianzas);
bool testMahalanobis(double alpha, double distMahalanobis);

vector < vector <String> > reconocer(String nomFich){
	vector < vector <String> > clasesReconocidas;
	Mat frame; Mat frameModificado;
	frame = imread(nomFich, CV_LOAD_IMAGE_GRAYSCALE);

	/* UMBRALIZAR */
	frameModificado = frame.clone();
	frameModificado = umbralizar(frameModificado);

	/* OBTENER BLOBS */
	vector <vector <Point> > contornos;
	vector<Vec4i> jerarquia;
	obtenerBlobs(frameModificado,contornos,jerarquia);

	/* OBTENER DESCRIPTORES */
	vector <double> areas;
	vector <double> perimetros;
	vector <double> mom1;
	vector <double> mom2;
	vector <double> mom3;
	obtenerArea(contornos,areas);
	obtenerPerimetro(contornos,perimetros);
	obtenerMomentos(contornos,mom1,mom2,mom3);

	for(int i = 0; i < areas.size(); i++){
		vector<double> descriptores;
		vector<String> clases;

		descriptores.push_back(areas[i]);
		descriptores.push_back(perimetros[i]);
		descriptores.push_back(mom1[i]);
		descriptores.push_back(mom2[i]);
		descriptores.push_back(mom3[i]);

		vector<String> objetos;

		ifstream ficheroObjetos;
		ficheroObjetos.open("./objetos.txt");

		std::stringstream line;
		String linea;
		String token;
		String nomObj;

		while(!ficheroObjetos.eof()){
			vector <double> medias;
			vector <double> varianzas;
			token.clear();
			ficheroObjetos >> token;
			if((token.compare(" ") != 0) && (token.length()!=0)){
				nomObj = token;
				token.clear();
				for(int j = 0; j < 5; j++){
					token.clear();
					ficheroObjetos >> token;
					medias.push_back(atof(token.c_str()));
					token.clear();
					ficheroObjetos >> token;
					varianzas.push_back(atof(token.c_str()));
					double descriptor = descriptores[j];
				}
				double distancia = distanciaMahalanobis(descriptores,medias,varianzas);
				if(testMahalanobis(0.05,distancia)){
					clases.push_back(nomObj);
				}
			}
		}
		if(clases.size() == 0){
			clases.push_back("objeto desconocido");
		}
		clasesReconocidas.push_back(clases);
	}
	return clasesReconocidas;
}

bool testMahalanobis(double alpha, double distMahalanobis){
	return distMahalanobis < 11.07;
}

double distanciaMahalanobis(vector<double> muestra, vector<double> medias, vector<double> varianzas){
	double distancia = 0.0;
	for(int i = 0; i < muestra.size(); i++){
		distancia = distancia + ( (muestra[i] - medias[i]) * (muestra[i] - medias[i]) ) / varianzas[i];
	}
	return distancia;
}
