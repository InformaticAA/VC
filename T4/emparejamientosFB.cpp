#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;
using namespace cv;

Mat emparejamientos(Mat &i1, Mat &i2);

Mat emparejamientos(Mat &i1, Mat &i2){
	Mat d1, d2, i_matches, inliers,result;
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
	for(unsigned int i = 0; i < matches.size(); i++){

		/* Aplica el filtro de ratio */
		if(matches[i][0].distance < 0.5*matches[i][1].distance){
			filtrados.push_back(matches[i][0]);
		}
	}

	for(unsigned int i = 0; i < filtrados.size(); i++){
		obj.push_back(kp1[ filtrados[i].queryIdx ].pt);
		scene.push_back(kp2[ filtrados[i].trainIdx ].pt);
	}

	Mat mask;
	Mat homography = findHomography(obj,scene,CV_RANSAC,3,mask);

	for(unsigned int i = 0; i < filtrados.size(); i++){
		if((int)mask.at<uchar>(i,0) == 1){
			ransac.push_back(filtrados[i]);
		}
	}

	vector <Point2f> corners;

	corners.push_back(Point2f(0,0));
	corners.push_back(Point2f(0,i1.rows));
	corners.push_back(Point2f(i1.cols,0));
	corners.push_back(Point2f(i1.cols,i1.rows));

	vector < Point2f > scene_corners;
	perspectiveTransform(corners, scene_corners, homography);

	float maxCols(0),maxRows(0),minCols(0),minRows(0);

	for(unsigned int i = 0; i < scene_corners.size(); i++){
		if(maxRows < scene_corners.at(i).y){
			maxRows = scene_corners.at(i).y;
		}
		if(minRows > scene_corners.at(i).y){
			minRows = scene_corners.at(i).y;
		}
		if(maxCols < scene_corners.at(i).x){
			maxCols = scene_corners.at(i).x;
		}
		if(minCols > scene_corners.at(i).x){
			minCols = scene_corners.at(i).x;
		}
	}

	for(unsigned int i = 0; i < scene_corners.size(); i++){
		scene_corners.at(i).x = scene_corners.at(i).x - minCols;
		scene_corners.at(i).y = scene_corners.at(i).y - minRows;
	}

	Mat newHomography = getPerspectiveTransform(corners,scene_corners);

//	/* Muestra los emparejamientos */
//	namedWindow("filtrados",1);
//	drawMatches(i1,kp1,i2,kp2,filtrados,i_matches);
//
//	/* Muestra los inliers */
//	namedWindow("inliers",1);
//	drawMatches(i1,kp1,i2,kp2,ransac,inliers);


	warpPerspective(i1,result,newHomography,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)));

//	cout << "No llega" <<endl;
//	namedWindow("Warp",1);
//	cout << "Jelou" << endl;
	Mat half(result,Rect(-minCols,-minRows,i2.cols,i2.rows));
//	cout << "half" << endl;
	i2.copyTo(half);

	return result;
}

