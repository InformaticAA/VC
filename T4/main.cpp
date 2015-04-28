#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;
using namespace cv;

extern Mat emparejamientos(Mat &i1, Mat &i2);

int main(int, char**){
	Mat i1,frame;
	namedWindow("Camara",1);

	cout << "Pulse INTRO para comenzar a crear el panorama..." << endl;

	VideoCapture cap(0);

	while(true){
		cap >> frame;
		imshow("Camara",frame);
		if(waitKey(30) == 13){
			break;
		}
	}

	cap >> i1;

	cout << "Grabando..." << endl;
	cout << "Presione INTRO para capturar imagen y añadirla al panorama" << endl;
	cout << "Presione ESCAPE para terminar la ejecución y mostrar el panorama" << endl;

	while(true){
		cap >> frame;
		imshow("Camara",frame);
		if(waitKey(30) == 13){
			cout << "Imagen añadida al panorama" << endl;
			cap >> frame;
			i1 = emparejamientos(i1,frame);
		}
		if(waitKey(1) == 27){
			break;
		}
	}
	cout << "Panorama completo guardado en Imagenes/Panorama.jpg" << endl;
	cout << "Mostrando panorama..." << endl;
	imwrite("Imagenes/Panorama.jpg",i1);
	imshow("Panorama", i1);
	waitKey(0);
}

