/*
 * aprendizaje.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al algoritmo de aprendizaje. Se encarga de
 * 				calcular los valores de los descriptores del
 * 				objeto encontrado en una imagen binaria y
 * 				actualizar un fichero con los datos de todos
 * 				los fichero aprendidos.
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
void aprender(String nomFich, String nomObj);
void calcularMediaYVarianza(vector<double> descriptores, double &media, double &varianza);
void guardarDatos(vector<double> descriptores, String nomObj);
void actualizarObjetos(String nuevaLinea, String nombreObjeto);

void aprender(String nomFich, String nomObj){
	Mat frame; Mat frameModificado;
	frame = imread(nomFich, CV_LOAD_IMAGE_GRAYSCALE);

	/* Umbraliza */
	frameModificado = frame.clone();
	frameModificado = umbralizar(frameModificado);

	/* Obtiene los blobs ya filtrados */
	vector <vector <Point> > contornos;
	vector<Vec4i> jerarquia;
	obtenerBlobs(frameModificado,contornos,jerarquia);

	/* Obtiene los descriptores */
	vector <double> areas;
	vector <double> perimetros;
	vector <double> mom1;
	vector <double> mom2;
	vector <double> mom3;
	obtenerArea(contornos,areas);
	obtenerPerimetro(contornos,perimetros);
	obtenerMomentos(contornos,mom1,mom2,mom3);

	vector<double> descriptores;
	descriptores.push_back(areas[0]);
	descriptores.push_back(perimetros[0]);
	descriptores.push_back(mom1[0]);
	descriptores.push_back(mom2[0]);
	descriptores.push_back(mom3[0]);

	/* Guarda los datos del nuevo objeto junto al resto */
	guardarDatos(descriptores,nomObj);

	vector<String> objetos;
	ifstream ficheroDatos;
	ficheroDatos.open("./datos.txt");

	String linea;
	String token;

	vector< vector< double > > descriptoresLeidos;
	for(int i = 0; i < 5; i++){
		descriptoresLeidos.push_back(vector<double>());
	}
	double descriptor;
	int encontrado = 0;

	/* Recorre un fichero, lee y guarda los datos de los objetos de la clase nomObj */
	while(!ficheroDatos.eof()){
		token.clear();
		ficheroDatos >> token;
		if((token.compare(" ") != 0) && (token.length()!=0)){
			if(token.compare(nomObj) == 0){
				encontrado = 1;

				/* Lee y almacena los descriptores de un objeto */
				for(int i = 0; i < 5; i++){
					token.clear();
					ficheroDatos >> token;
					descriptor = atof(token.c_str());
					descriptoresLeidos[i].push_back(descriptor);
				}

				/* Pasa a la siguiente linea */
				getline(ficheroDatos,linea);
			}
			else{

				/* Pasa a la siguiente linea */
				getline(ficheroDatos,linea);
			}
		}
	}

	double media;
	double varianza;
	std::stringstream line;
	line << nomObj;

	/* Calcula la media y varianza para los distintos descriptores de una clase */
	for(int i = 0; i < descriptoresLeidos.size(); i++){
		calcularMediaYVarianza(descriptoresLeidos[i],media,varianza);
		line << " " << media << " " << varianza;
	}
	linea = line.str();

	/* Escribe en un fichero los datos actualizados de una clase */
	actualizarObjetos(linea,nomObj);
}

void calcularMediaYVarianza(vector<double> descriptores, double &media, double &varianza){
	double factorReg = 0.03;
	int N = descriptores.size();
	media = 0;
	varianza = 0;

	/* Calcula la media */
	for (int i = 0; i < N; i++) {
		media = media + descriptores[i];
	}
	media = media/N;

	/* Calcula la varianza */
	if (N != 1){
		for (int j = 0; j < N; j++) {
			varianza = varianza + (descriptores[j] - media) * (descriptores[j] - media);
		}
		varianza = varianza/(N-1);
	}

	/* Regulariza la varianza */
	varianza = (varianza * (N-1))/N + (media * factorReg * media * factorReg )/N;
}

void guardarDatos(vector<double> descriptores, String nomObj){
	fstream ficheroDatos;
	ficheroDatos.open("./datos.txt", fstream::out | fstream::app);
	std::stringstream line;
	String linea;
	line << nomObj;

	/* Organiza los datos de la manera adecuada */
	for(int i = 0; i < descriptores.size(); i++){
		line << " " << descriptores[i];
	}
	linea = line.str();

	/* Escribe los datos en el fichero */
	ficheroDatos << linea << endl;
	ficheroDatos.close();
}

void actualizarObjetos(String nuevaLinea, String nombreObjeto){
	ifstream ficheroObjetos;
	ficheroObjetos.open("./objetos.txt");
	vector<String> objetos;
	std::stringstream line;
	String token;
	String linea;

	double mA,vA,mP,vP,m1,v1,m2,v2,m3,v3;

	int encontrado = 0;
	while(ficheroObjetos >> token >> mA >> vA >> mP >> vP >> m1 >> v1 >> m2 >> v2 >> m3 >> v3){
		if((token.compare(" ") != 0) && (token.length()!=0)){
			if(token.compare(nombreObjeto) == 0){
				encontrado = 1;
				objetos.push_back(nuevaLinea);
			}
			else{
				line.str(string());
				line.clear();
				line << token << " " << mA << " " << vA << " " << mP << " " << vP << " " <<
						m1 << " " << v1 << " " << m2 << " " << v2 << " " << m3 << " " << v3;
				linea = line.str();
				objetos.push_back(linea);
			}
		}
	}
	if(!encontrado){
		objetos.push_back(nuevaLinea);
	}

	/* Escribe el nuevo contenido del fichero */
	ofstream ficheroObjetosW;
	ficheroObjetosW.open("./objetos.txt");
	for(int i = 0; i < objetos.size(); i++){
		ficheroObjetosW << objetos[i] << endl;
	}
}
