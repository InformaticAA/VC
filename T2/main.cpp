/*
 * main.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al programa principal. Se encarga de realizar la
 * 				operacion solicitada por el usuario, ya sea
 * 				entrenar el sistema con una nueva imagen o
 * 				reconocer los objetos encontrados en otra.
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace std;
using namespace cv;

extern Mat umbralizar(cv::Mat &img);
extern void obtenerBlobs(cv::Mat &img,vector <vector <Point> > &contornos,vector<Vec4i> &jerarquia);
extern void aprender(String nomFich, String nomObj);
extern vector < vector <String> > reconocer(String nomFich);
void mostrar_info_reconocida(vector <vector <String> > reconocimiento, Mat imagen);
void aprenderAuto();

int main(int, char**){

	/* Crea el fichero objetos.txt si no estaba creado */
	FILE *fichero;
	fichero = fopen("objetos.txt","a");
	fclose(fichero);
	fichero = fopen("datos.txt","a");
	fclose(fichero);

	int opcion;
	String nombreFichero;
	String nombreObjeto;
	bool continuar = true;

	while (continuar) {
		cout << "1)Entrenar." << endl;
		cout << "2)Reconocer." << endl;
		cout << "3)Salir." << endl;
		cout << "Elija una opción: ";
		cin >> opcion;
		cout << "==========================" << endl;
		if (opcion == 1){
			cout << "Manual(1) o Automático(2): ";
			cin >> opcion;

			if(opcion == 1){
				cout << "Ruta relativa del fichero: ";
				cin >> nombreFichero;
				cout << "Nombre del objeto: ";
				cin >> nombreObjeto;

				/* Aprende del fichero proporcionado por el usuario */
				aprender(nombreFichero, nombreObjeto);

				cout << "Fichero " << nombreFichero << " aprendido." << endl;

			} else if(opcion == 2){
				aprenderAuto();
				cout << "Aprendizaje automático finalizado." << endl;
			}

		} else if (opcion == 2){
			cout << "Ruta relativa del fichero: ";
			cin >> nombreFichero;

			/* Reconoce los objetos en el fichero y los asocia con las clases aprendidas */
			Mat img = imread(nombreFichero, CV_LOAD_IMAGE_GRAYSCALE);
			Mat img2 = img.clone();
			Mat imgRecon = umbralizar(img2);

			namedWindow("Original",1);
			imshow("Original", img);
			waitKey(0);
			namedWindow("Umbralizada",1);
			imshow("Umbralizada", imgRecon);
			waitKey(0);
			vector <vector < String > > reconocimiento = reconocer(nombreFichero);

			mostrar_info_reconocida(reconocimiento,imgRecon);

			cout << "Fichero " << nombreFichero << " reconocido." << endl;

		} else if (opcion == 3){
			continuar = false;
			cout << "Fin del programa." << endl;
		} else {
			continuar = false;
			cout << "Respuesta no válida. Fin del programa." << endl;
		}
		if (continuar) {
			cout << "¿Quiere seguir realizando operaciones? (1/2): ";
			cin >> opcion;
			if (opcion == 2) {
				continuar = false;
				cout << "Fin del programa." << endl;
			}
		}
	}
}

void mostrar_info_reconocida(vector <vector <String> > reconocimiento, Mat imagen){
	vector< vector <Point> > contornos;
	vector< Vec4i> jerarquia;

	obtenerBlobs(imagen,contornos,jerarquia);

	for(int i = 0; i < reconocimiento.size(); i++){
		Scalar blanco = Scalar(255,255,255);
		Mat dibujo = Mat::zeros(imagen.size(), CV_8UC3);
		drawContours(dibujo, contornos, i, blanco,CV_FILLED, 8);
		if(reconocimiento[i].size() > 1){
			putText(dibujo,"Duda entre clases",Point(20,40),1,2,Scalar(0,0,255));
		}
		else{
			putText(dibujo,reconocimiento[i][0],Point(20,40),1,2,Scalar(0,0,255));
		}
		namedWindow("Blobs",1);
		imshow("Blobs",dibujo);

		if (reconocimiento[i].size() == 1) {

			/* El objeto ha pasado el test para una sola clase */
			cout << "Es un " << reconocimiento[i][0] << "." << endl;
		} else {

			/* El objeto ha pasado el test para mas de una clase */
			cout << "El objeto se confunde entre varias clases: " << endl;
			for(int j = 0; j < reconocimiento[i].size(); j++){
				cout << "-" << reconocimiento[i][j] << endl;
			}
		}
		waitKey(0);
	}
	destroyAllWindows();
}

void aprenderAuto(){
	aprender("Imagenes/circulo1.pgm", "circulo");
	aprender("Imagenes/circulo2.pgm", "circulo");
	aprender("Imagenes/circulo3.pgm", "circulo");
	aprender("Imagenes/circulo4.pgm", "circulo");
	aprender("Imagenes/circulo5.pgm", "circulo");

	aprender("Imagenes/rectangulo1.pgm", "rectangulo");
	aprender("Imagenes/rectangulo2.pgm", "rectangulo");
	aprender("Imagenes/rectangulo3.pgm", "rectangulo");
	aprender("Imagenes/rectangulo4.pgm", "rectangulo");
	aprender("Imagenes/rectangulo5.pgm", "rectangulo");

	aprender("Imagenes/rueda1.pgm", "rueda");
	aprender("Imagenes/rueda2.pgm", "rueda");
	aprender("Imagenes/rueda3.pgm", "rueda");
	aprender("Imagenes/rueda4.pgm", "rueda");
	aprender("Imagenes/rueda5.pgm", "rueda");

	aprender("Imagenes/triangulo1.pgm", "triangulo");
	aprender("Imagenes/triangulo2.pgm", "triangulo");
	aprender("Imagenes/triangulo3.pgm", "triangulo");
	aprender("Imagenes/triangulo4.pgm", "triangulo");
	aprender("Imagenes/triangulo5.pgm", "triangulo");

	aprender("Imagenes/vagon1.pgm", "vagon");
	aprender("Imagenes/vagon2.pgm", "vagon");
	aprender("Imagenes/vagon3.pgm", "vagon");
	aprender("Imagenes/vagon4.pgm", "vagon");
	aprender("Imagenes/vagon5.pgm", "vagon");
}
