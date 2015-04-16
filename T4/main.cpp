#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <math.h>

using namespace std;
using namespace cv;

extern void emparejamientos(Mat &i1, Mat &i2);

int main(int, char**){

	cout << "Pre imagenes" << endl;

	Mat i1 = imread("Imagenes/castle1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat i2 = imread("Imagenes/castle2.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	cout << " imagenes bien " << endl;

	emparejamientos(i1,i2);
}

