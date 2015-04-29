#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;
using namespace cv;

extern Mat panorama(Mat &i1, Mat &i2, int info);
extern void comparativa(Mat &i1, Mat &i2);

int main(int, char**){
	bool continuar = true;
	int opcion;
	while(continuar){
		cout << "==================================" << endl;
		cout << "Operaciones disponibles:" << endl;
		cout << "(1) Panorama con imágenes de disco" << endl;
		cout << "(2) Panorama con imágenes en vivo" << endl;
		cout << "(3) Comparativa detectores y descriptores" << endl;
		cout << "Elija una opción (1/2/3): ";
		cin >> opcion;
		cout << "==================================" <<endl;
		if(opcion == 1){
			String ruta1,ruta2;
			int info, mas;
			Mat i1,i2;
			cout << "Introduzca ruta 1ª imagen: ";
			cin >> ruta1;
			i1 = imread(ruta1,CV_LOAD_IMAGE_COLOR);
			cout << "Introduzca ruta 2ª imagen: ";
			cin >> ruta2;
			i2 = imread(ruta2,CV_LOAD_IMAGE_COLOR);
			cout << "¿Desea mostrar los emparejamientos? (1/2): ";
			cin >> info;
			bool moar = true;
			while(moar){
				i2 = panorama(i1,i2,info);
				cout << "¿Desea agregar otra imagen al panorama? (1/2): ";
				cin >> mas;
				if(mas == 1){
					cout << "Introduzca ruta siguiente imagen: ";
					cin >> ruta2;
					i1 = imread(ruta2,CV_LOAD_IMAGE_COLOR);
					destroyAllWindows();
				}
				else{
					moar = false;
				}
			}
			destroyAllWindows();
			cout << "Panorama completo guardado en Imagenes/Panorama.jpg" << endl;
			cout << "Mostrando panorama..." << endl;
			imwrite("Imagenes/Panorama.jpg",i2);
			imshow("Panorama", i2);
			waitKey(0);
			destroyAllWindows();
			cout << "============================================== " << endl;
			cout << "¿Desea continuar realizando operaciones? (1/2) " << endl;			cin >> opcion;
			if(opcion == 2){
				continuar = false;
			}
		}
		else if(opcion==2){
			Mat i1,frame;
			namedWindow("Camara",1);

			cout << "Pulse INTRO para comenzar a crear el panorama..." << endl;

			VideoCapture cap(0);

			while(true){
				cap >> frame;
				flip(frame,frame,1);
				imshow("Camara",frame);
				if(waitKey(30) == 13){
					break;
				}
			}

			cap >> i1;
			flip(i1,i1,1);

			cout << "Grabando..." << endl;
			cout << "Presione INTRO para capturar imagen y agregarla al panorama" << endl;
			cout << "Presione ESCAPE para terminar la ejecución y mostrar el panorama" << endl;
			cout << "================================================================" << endl;
			while(true){
				cap >> frame;
				flip(frame,frame,1);
				imshow("Camara",frame);
				int wait = waitKey(20);
				if(wait == 13){
					cout << "Imagen tomada" << endl;
					cap >> frame;
					flip(frame,frame,1);
					i1 = panorama(frame,i1,2);
				}
				if(wait == 27){
					break;
				}
			}
			destroyAllWindows();
			cap.release();
			cout << "Panorama completo guardado en Imagenes/Panorama.jpg" << endl;
			cout << "Mostrando panorama..." << endl;
			imwrite("Imagenes/Panorama.jpg",i1);
			imshow("Panorama", i1);
			waitKey(0);
			destroyAllWindows();
			cout << "============================================== " << endl;
			cout << "¿Desea continuar realizando operaciones? (1/2) " << endl;
			cin >> opcion;
			if(opcion == 2){
				continuar = false;
			}
		}
		else if(opcion == 3){
			Mat i1 = imread("Imagenes/castle1.jpg",CV_LOAD_IMAGE_COLOR);
			Mat i2 = imread("Imagenes/castle2.jpg",CV_LOAD_IMAGE_COLOR);
			comparativa(i1,i2);
			cout << "¿Desea continuar realizando operaciones? (1/2) " << endl;
			cin >> opcion;
			if(opcion == 2){
				continuar = false;
			}
		}
		else{
			cerr << "ERROR: opción inválida" << endl;
		}
	}
	cout << "Programa finalizado." << endl;
}

