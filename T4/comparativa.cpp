/*
 * comparativa.cpp
 *
 *  Created on: 28/04/2015
 *      Author: Alejandro
 */

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include <windows.h>

using namespace std;
using namespace cv;

void comparativa(Mat &i1, Mat &i2);
void comparativaSURF(Mat &i1, Mat &i2);
void comparativaORB(Mat &i1, Mat &i2);
void comparativaSIFT(Mat &i1, Mat &i2);
void comparativaBRISK(Mat &i1, Mat &i2);
void comparativaSURFFLANN(Mat &i1, Mat &i2);
void StartCounter();
double GetCounter();

double PCFreq = 0.0;
__int64 CounterStart = 0;

void StartCounter()
{
    LARGE_INTEGER li;
    if(!QueryPerformanceFrequency(&li))
	cout << "QueryPerformanceFrequency failed!\n";

    PCFreq = double(li.QuadPart)/1000.0;

    QueryPerformanceCounter(&li);
    CounterStart = li.QuadPart;
}
double GetCounter()
{
    LARGE_INTEGER li;
    QueryPerformanceCounter(&li);
    return double(li.QuadPart-CounterStart)/PCFreq;
}

void comparativa(Mat &i1, Mat &i2){

	cout << "Ejecutando con ORB..." << endl;
	comparativaORB(i1,i2);
	cout << "===========================" << endl;
	cout << "Ejecutando con SIFT..." <<endl;
	comparativaSIFT(i1,i2);
	cout << "===========================" << endl;
	cout << "Ejecutando con BRISK..." <<endl;
	comparativaBRISK(i1,i2);
	cout << "===========================" << endl;
	cout << "Ejecutando con SURF..." << endl;
	comparativaSURF(i1,i2);
	cout << "===========================" << endl;
	cout << "Ejecutando con SURF (FLANN)" << endl;
	comparativaSURFFLANN(i1,i2);

}

void comparativaSURF(Mat &i1, Mat &i2){
	double tiempo = 0;
	for(int i = 0; i < 5; i++){
		Mat i1g, i2g, d1, d2, i_matches, inliers, result;
		cvtColor(i1,i1g,CV_BGR2GRAY);
		cvtColor(i2,i2g,CV_BGR2GRAY);
		vector< KeyPoint > kp1, kp2;
		vector< vector <DMatch> > matches;
		vector < DMatch > filtrados, ransac;
		vector< Point2f > obj, scene;

		StartCounter();

		/* Detectar puntos de interes */
		SurfFeatureDetector detector(400);
		detector.detect(i1g,kp1);
		detector.detect(i2g,kp2);

		/* Obtiene los descriptores de cada punto de interes */
		SurfDescriptorExtractor extractor;
		extractor.compute(i1g,kp1,d1);
		extractor.compute(i2g,kp2,d2);

		/* Realiza los emparejamientos, con filtro de ratio */
		BFMatcher matcher(NORM_L2);
		matcher.knnMatch(d1,d2,matches,2);
		for(unsigned int i = 0; i < matches.size(); i++){

			/* Aplica el filtro de ratio */
			if(matches[i][0].distance < 0.5*matches[i][1].distance){
				filtrados.push_back(matches[i][0]);
			}
		}
		double actual = GetCounter();
		tiempo = tiempo + actual;
	}
	cout << "Media: " << tiempo/5 << " ms" << endl;
}

void comparativaSURFFLANN(Mat &i1, Mat &i2){
	double tiempo = 0;
	for(int i = 0; i < 5; i++){
		Mat i1g, i2g, d1, d2, i_matches, inliers, result;
		cvtColor(i1,i1g,CV_BGR2GRAY);
		cvtColor(i2,i2g,CV_BGR2GRAY);
		vector< KeyPoint > kp1, kp2;
		vector< vector <DMatch> > matches;
		vector < DMatch > filtrados, ransac;
		vector< Point2f > obj, scene;

		StartCounter();

		/* Detectar puntos de interes */
		SurfFeatureDetector detector(400);
		detector.detect(i1g,kp1);
		detector.detect(i2g,kp2);

		/* Obtiene los descriptores de cada punto de interes */
		SurfDescriptorExtractor extractor;
		extractor.compute(i1g,kp1,d1);
		extractor.compute(i2g,kp2,d2);

		/* Realiza los emparejamientos, con filtro de ratio */
		FlannBasedMatcher matcher;
		matcher.knnMatch(d1,d2,matches,2);
		for(unsigned int i = 0; i < matches.size(); i++){

			/* Aplica el filtro de ratio */
			if(matches[i][0].distance < 0.5*matches[i][1].distance){
				filtrados.push_back(matches[i][0]);
			}
		}
		double actual = GetCounter();
		tiempo = tiempo + actual;
	}
	cout << "Media: " << tiempo/5 << " ms" << endl;
}

void comparativaORB(Mat &i1, Mat &i2){
	double tiempo = 0;
	for(int i = 0; i < 5; i++){
		Mat i1g, i2g, d1, d2, i_matches, inliers, result;
		cvtColor(i1,i1g,CV_BGR2GRAY);
		cvtColor(i2,i2g,CV_BGR2GRAY);
		vector< KeyPoint > kp1, kp2;
		vector< vector <DMatch> > matches;
		vector < DMatch > filtrados, ransac;
		vector< Point2f > obj, scene;

		StartCounter();

		/* Detectar puntos de interes */
		FeatureDetector *detector = new ORB();
		detector->detect(i1g,kp1);
		detector->detect(i2g,kp2);

		/* Obtiene los descriptores de cada punto de interes */
		DescriptorExtractor *extractor = new ORB();
		extractor->compute(i1g,kp1,d1);
		extractor->compute(i2g,kp2,d2);
		/* Realiza los emparejamientos, con filtro de ratio */
		BFMatcher matcher(NORM_HAMMING);
		matcher.knnMatch(d1,d2,matches,2);
		for(unsigned int i = 0; i < matches.size(); i++){

			/* Aplica el filtro de ratio */
			if(matches[i][0].distance < 0.5*matches[i][1].distance){
				filtrados.push_back(matches[i][0]);
			}
		}
		double actual = GetCounter();
		tiempo = tiempo + actual;

	}
	cout << "Media: " << tiempo/5 << " ms" << endl;
}

void comparativaSIFT(Mat &i1, Mat &i2){
	double tiempo = 0;
	for(int i = 0; i < 5; i++){
		Mat i1g, i2g, d1, d2, i_matches, inliers, result;
		cvtColor(i1,i1g,CV_BGR2GRAY);
		cvtColor(i2,i2g,CV_BGR2GRAY);
		vector< KeyPoint > kp1, kp2;
		vector< vector <DMatch> > matches;
		vector < DMatch > filtrados, ransac;
		vector< Point2f > obj, scene;

		StartCounter();

		/* Detectar puntos de interes */
		FeatureDetector *detector = new SIFT();
		detector->detect(i1g,kp1);
		detector->detect(i2g,kp2);

		/* Obtiene los descriptores de cada punto de interes */
		DescriptorExtractor *extractor = new SIFT();
		extractor->compute(i1g,kp1,d1);
		extractor->compute(i2g,kp2,d2);
		/* Realiza los emparejamientos, con filtro de ratio */
		BFMatcher matcher(NORM_L2);
		matcher.knnMatch(d1,d2,matches,2);
		for(unsigned int i = 0; i < matches.size(); i++){

			/* Aplica el filtro de ratio */
			if(matches[i][0].distance < 0.5*matches[i][1].distance){
				filtrados.push_back(matches[i][0]);
			}
		}

		double actual = GetCounter();
		tiempo = tiempo + actual;

	}
	cout << "Media: " << tiempo/5 << " ms" << endl;
}

void comparativaBRISK(Mat &i1, Mat &i2){
	double tiempo = 0;
	for(int i = 0; i < 5; i++){
		Mat i1g, i2g, d1, d2, i_matches, inliers, result;
		cvtColor(i1,i1g,CV_BGR2GRAY);
		cvtColor(i2,i2g,CV_BGR2GRAY);
		vector< KeyPoint > kp1, kp2;
		vector< vector <DMatch> > matches;
		vector < DMatch > filtrados, ransac;
		vector< Point2f > obj, scene;

		StartCounter();

		/* Detectar puntos de interes */
		FeatureDetector *detector = new BRISK();
		detector->detect(i1g,kp1);
		detector->detect(i2g,kp2);

		/* Obtiene los descriptores de cada punto de interes */
		DescriptorExtractor *extractor = new BRISK();
		extractor->compute(i1g,kp1,d1);
		extractor->compute(i2g,kp2,d2);
		/* Realiza los emparejamientos, con filtro de ratio */
		BFMatcher matcher(NORM_HAMMING);
		matcher.knnMatch(d1,d2,matches,2);
		for(unsigned int i = 0; i < matches.size(); i++){

			/* Aplica el filtro de ratio */
			if(matches[i][0].distance < 0.5*matches[i][1].distance){
				filtrados.push_back(matches[i][0]);
			}
		}
		double actual = GetCounter();
		tiempo = tiempo + actual;

	}
	cout << "Media: " << tiempo/5 << " ms" << endl;
}

