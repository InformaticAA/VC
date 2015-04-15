/*
 * main.cpp
 *
 * Autores: Alejandro Marquez Ferrer (566400)
 * 			Alejandro Royo Amondarain (560285)
 *
 * Descripcion: Este fichero contiene el codigo correspondiente
 * 				al programa principal. Se encarga de mostrar la
 * 				imagen original (capturada por la camara) junto
 * 				a la imagen modificada por el efecto elegido en
 * 				cada momento por el usuario en tiempo real.
 *
 */
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void aplicarEfecto(int tecla, cv::Mat &img);
void mostrarMenu();

extern Mat distorsion(cv::Mat &img, int tipo, double modificador);
extern void cambiarContrasteManual(cv::Mat &img, double modificacion);
extern Mat ecualizacionHistograma(cv::Mat &img);
extern void reducirColor(cv::Mat &img, int div);
extern Mat alien(cv::Mat &img, int color);

int main(int, char**){

	VideoCapture cap(0);

	if(!cap.isOpened())
		return -1;

	namedWindow("Original",1);
	namedWindow("Modificada",1);
	mostrarMenu();

	while (true) {

		/* Obtiene un nuevo frame de la camara */
		Mat frame, frameModificado;
		cap >> frame;

		/* Operaciones con el frame */
		flip(frame, frame, 1); // Corrige el efecto espejo
		frameModificado = frame.clone();

		/* Pulsacion de tecla */
		int tecla = waitKey(30);
		aplicarEfecto(tecla,frameModificado);

		/* Muestra el frame original junto al procesado */
		imshow("Original", frame);
		imshow("Modificada",frameModificado);

		if(waitKey(30) == 27) break;
	}
	cap.release();
	return 0;
}

void aplicarEfecto(int tecla, cv::Mat &img){
	static int div = 64;
	static int color = 0;
	static int estado = 0;
	static int modoContraste = 1;
	static int modoDistorsion = 1;
	static double cambioContraste = 1;
	static double cambioDistorsion = 0;

	if (estado == 0) { //Sin modificar

	} else if (estado == 1){ // Efecto alien
		if(tecla == 'r'){
			color = 1;
		} else if(tecla == 'a') {
			color = 2;
		} else if(tecla == 'v'){
			color = 3;
		}
		img = alien(img,color);
	} else if (estado == 2){ // Cambio contraste

		if(modoContraste == 1) { // Contraste manual
			if(tecla == '+') {
				cambioContraste = cambioContraste + 0.1;
			} else if (tecla == '-' && cambioContraste > 0.3) {
				cambioContraste = cambioContraste - 0.1;
			}
			cambiarContrasteManual(img, cambioContraste);
		} else if (modoContraste == 2) { // Ecualizacion de histograma
			img = ecualizacionHistograma(img);
		}

		if(tecla == 'm'){
			modoContraste = 1;
		} else if(tecla == 'e') {
			modoContraste = 2;
		}
	} else if (estado == 3){ // Distorsion imagen
		if(modoDistorsion == 1) { // Distorsion de cojin
			if (tecla == '+') {
				cambioDistorsion = cambioDistorsion + 0.00000000001;
			} else if (tecla == '-' && cambioDistorsion > 0) {
				cambioDistorsion = cambioDistorsion - 0.00000000001;
			}
			img = distorsion(img,modoDistorsion,cambioDistorsion);
		} else if (modoDistorsion == 2) { // Distorsion de barril
			if (tecla == '+') {
				cambioDistorsion = cambioDistorsion - 0.00000000001;
			} else if (tecla == '-' && cambioDistorsion < 0) {
				cambioDistorsion = cambioDistorsion + 0.00000000001;
			}
			img = distorsion(img,modoDistorsion,cambioDistorsion);
		}

		if(tecla == 'c') {
			modoDistorsion = 1;
			cambioDistorsion = 0;
		} else if (tecla == 'b') {
			modoDistorsion = 2;
			cambioDistorsion = 0;
		}
	} else if (estado == 4){ // Efecto poster
		if(tecla == '+' && div < 128) {
			div = div * 2;
		} else if (tecla == '-' && div > 1) {
			div = div / 2;
		}
		reducirColor(img,div);
	}

	/* Comprobacion teclas */
	if(tecla == '1'){
		estado = 1;
	} else if (tecla == '2') {
		estado = 2;
	} else if (tecla == '3') {
		estado = 3;
	} else if (tecla == '4') {
		estado = 4;
	}
}

void mostrarMenu(){
	cout << "Efectos disponibles (Pulsa ESC para salir):" << endl;
	cout << "  1)Alien: Azul(a), Rojo(r), Verde(v)" << endl;
	cout << "  2)Contraste: Manual(m [+ -]), Ecualizacion(e)" << endl;
	cout << "  3)Distorsion: Cojin(c [+ -]), Barril(b [+ -])" << endl;
	cout << "  4)Poster [+ -]" << endl;
}
