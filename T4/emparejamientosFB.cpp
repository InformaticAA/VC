#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;
using namespace cv;

void emparejamientos(Mat &i1, Mat &i2);

void emparejamientos(Mat &i1, Mat &i2){
	Mat d1, d2, i_matches, inliers;
	vector< KeyPoint > kp1, kp2;
	vector< vector <DMatch> > matches;
	vector < DMatch > filtrados, ransac;
	vector< Point2f > obj, scene;

	/* Detectar puntos de interes */
	SurfFeatureDetector detector(400);
	detector.detect(i1,kp1);
	detector.detect(i2,kp2);

	/* Obtiene los descriptores de cada punto de interes */
	SurfDescriptorExtractor extractor;
	extractor.compute(i1,kp1,d1);
	extractor.compute(i2,kp2,d2);

	/* Realiza los emparejamientos, con filtro de ratio */
	BFMatcher matcher(NORM_L2);
	matcher.knnMatch(d1,d2,matches,2);
	for(int i = 0; i < matches.size(); i++){

		/* Aplica el filtro de ratio */
		if(matches[i][0].distance < 0.5*matches[i][1].distance){
			filtrados.push_back(matches[i][0]);
		}
	}

	for(int i = 0; i < filtrados.size(); i++){
		obj.push_back(kp1[ filtrados[i].queryIdx ].pt);
		scene.push_back(kp2[ filtrados[i].trainIdx ].pt);
	}

	Mat mask;
	Mat homography = findHomography(obj,scene,CV_RANSAC,3,mask);

	for(int i = 0; i < filtrados.size(); i++){
		if((int)mask.at<uchar>(i,0) == 1){
			ransac.push_back(filtrados[i]);
		}
	}

	/* Muestra los emparejamientos */
	namedWindow("filtrados",1);
	drawMatches(i1,kp1,i2,kp2,filtrados,i_matches);
	imshow("filtrados",i_matches);


	/* Muestra los inliers */
	namedWindow("inliers",1);
	drawMatches(i1,kp1,i2,kp2,ransac,inliers);
	imshow("inliers",inliers);
	waitKey(0);
}

