#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>

using namespace std;
using namespace cv;

Mat panorama(Mat &i1, Mat &i2, int info);

Mat panorama(Mat &i1, Mat &i2, int info){
	Mat i1g, i2g, d1, d2, i_matches, inliers, result;
	cvtColor(i1,i1g,CV_BGR2GRAY);
	cvtColor(i2,i2g,CV_BGR2GRAY);
	vector< KeyPoint > kp1, kp2;
	vector< vector <DMatch> > matches;
	vector < DMatch > filtrados, ransac;
	vector< Point2f > obj, scene;

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
	corners.push_back(Point2f(0,i1g.rows));
	corners.push_back(Point2f(i1g.cols,0));
	corners.push_back(Point2f(i1g.cols,i1g.rows));

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

//	for(unsigned int i = 0; i < scene_corners.size(); i++){
//		scene_corners.at(i).x = scene_corners.at(i).x - minCols;
//		scene_corners.at(i).y = scene_corners.at(i).y - minRows;
//	}
//
//	Mat newHomography = getPerspectiveTransform(corners,scene_corners);

	Mat euclid = Mat::eye(3,3,homography.type());
	euclid.at<double>(0,2) = -minCols;
	euclid.at<double>(1,2) = -minRows;

	if(info == 1){
		/* Muestra los emparejamientos */
		namedWindow("Emparejamientos filtrados",1);
		drawMatches(i1g,kp1,i2g,kp2,filtrados,i_matches);
		imshow("Emparejamientos filtrados", i_matches);

		/* Muestra los inliers */
		namedWindow("Inliers",1);
		drawMatches(i1g,kp1,i2g,kp2,ransac,inliers);
		imshow("Inliers", inliers);
		waitKey(0);
	}

//	Mat result = Mat(Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),CV_32F);
//	Mat result = Mat::zeros(Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),CV_32F);
//	warpPerspective(i1,result,newHomography,result.size(),INTER_LINEAR,BORDER_TRANSPARENT,0);
//	Mat half(result,Rect(-minCols,-minRows,i2.cols,i2.rows));
//	i2.copyTo(half);
//	Mat temp;
//	imshow("Half",half);
//	waitKey(0);
//	waitKey(0);
//	warpPerspective(i2,result,newHomography,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),INTER_LINEAR,BORDER_CONSTANT,0);
//	imshow("Result",result);
//	waitKey(0);
//	imshow("Result",result);
//	waitKey(0);

//
//	Mat result;
//	warpPerspective(i1,result,newHomography,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),INTER_LINEAR,BORDER_CONSTANT,0);
//	Mat half(result,Rect(-minCols,-minRows,i2.cols,i2.rows));


	warpPerspective(i2,result,euclid,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),INTER_LINEAR,BORDER_CONSTANT,0);
	warpPerspective(i1,result,euclid*homography,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)),INTER_LINEAR,BORDER_TRANSPARENT,0);

//	Mat tmp,alpha,dst;
//
//	threshold(i2g,alpha,100,255,THRESH_BINARY);
//
//	Mat rgb[3];
//	split(i2,rgb);
//
//	Mat rgba[4]={rgb[0],rgb[1],rgb[2],alpha};
//	merge(rgba,4,dst);
//
//	i2.copyTo(half);


//	imshow("half",half);
//	waitKey(0);
//	imshow("Temp",temp);
//	waitKey(0);
//	temp.copyTo(result);

	return result;
}

