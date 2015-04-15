/*
 * main.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al programa principal. Se encarga de realizar la
 * 				operacion solicitada por el usuario, ya sea
 * 				ejecutar el apartado 1, 2 o el opcional (solo
 * 				para una imagen).
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;
using namespace cv;

extern void punto1();
extern void punto2(String imagen);
extern void puntoOpcional();

int main(int, char**){
	int opcion;
	String imagen;
	bool continuar = true;

	while(continuar) {
		cout << "1)Primer apartado" << endl;
		cout << "2)Segundo apartado" << endl;
		cout << "3)Apartado opcional" << endl;
		cout << "Elija una opción: ";
		cin >> opcion;

		if (opcion == 1) {
			punto1();
		}
		else if (opcion == 2){
			cout << "Introduzca la ruta de la imagen: ";
			cin >> imagen;
			punto2(imagen);
		}
		else if (opcion == 3) {
			puntoOpcional();
		}
		else {
			cout << "Opcion invalida" << endl;
			break;
		}
		destroyAllWindows();
		cout << "¿Continuar? (1/2): ";
		cin >> opcion;
		if (opcion == 2) {
			continuar = false;
		}
	}
}

