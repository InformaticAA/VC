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

	Mat i1 = imread("Imagenes/result2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
	Mat i2 = imread("Imagenes/1.jpg", CV_LOAD_IMAGE_GRAYSCALE);

	emparejamientos(i1,i2);
}

