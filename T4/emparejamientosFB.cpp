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
		cout << "X: " << corners.at(i).x << ",X': " << scene_corners.at(i).x << " - ";
		cout << "Y: " << corners.at(i).y << ",Y': " << scene_corners.at(i).y << endl;
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

//	vector <Point2f> obj_corners;
//
//	obj_corners.push_back(Point2f(-minCols,-minRows));
//	obj_corners.push_back(Point2f(-minCols,i2.rows-minRows));
//	obj_corners.push_back(Point2f(i2.cols-minCols,-minRows));
//	obj_corners.push_back(Point2f(i2.cols-minCols,i2.rows-minRows));
////
//	cout << "===================" <<endl;
//	cout << "Min rows : " << minRows << endl;
//	cout << "Min cols : " << minCols <<endl;
//
//	cout << "Max rows : " << maxRows << endl;
//	cout << "Max cols : " << maxCols << endl;


	/* Muestra los emparejamientos */
	namedWindow("filtrados",1);
	drawMatches(i1,kp1,i2,kp2,filtrados,i_matches);
	imshow("filtrados",i_matches);

	/* Muestra los inliers */
	namedWindow("inliers",1);
	drawMatches(i1,kp1,i2,kp2,ransac,inliers);
	imshow("inliers",inliers);
	waitKey(0);

	cout << "LLega" <<endl;
	cout << "I2.rows - minRows: " << i2.rows - minRows << " - " << "MaxRows " << maxRows << endl;

	/* Calcular rectangulo inscrito */

//	cout << "Esquinas" << endl;
//	cout << "========" << endl;
//	cout << "Scene_object: P1 = (" << scene_corners[0].x << "," << scene_corners[0].y << ")" << endl;
//	cout << "Scene_object: P2 = (" << scene_corners[1].x << "," << scene_corners[1].y << ")" << endl;
//	cout << "Scene_object: P3 = (" << scene_corners[2].x << "," << scene_corners[2].y << ")" << endl;
//	cout << "Scene_object: P4 = (" << scene_corners[3].x << "," << scene_corners[3].y << ")" << endl;
//	cout << "Obj_object: P1 = (" << obj_corners[0].x << "," << obj_corners[0].y << ")" << endl;
//	cout << "Obj_object: P2 = (" << obj_corners[1].x << "," << obj_corners[1].y << ")" << endl;
//	cout << "Obj_object: P3 = (" << obj_corners[2].x << "," << obj_corners[2].y << ")" << endl;
//	cout << "Obj_object: P4 = (" << obj_corners[3].x << "," << obj_corners[3].y << ")" << endl;
//
//	int x1 = min(max(scene_corners[0].x,scene_corners[1].x),
//				 max(obj_corners[0].x,obj_corners[1].x));
//	int y1 = max(max(scene_corners[0].y, scene_corners[2].y),
//				 max(obj_corners[0].y, obj_corners[2].y));
//
//	int x2 = max(min(scene_corners[2].x,scene_corners[3].x),
//				 min(obj_corners[2].x,obj_corners[3].x));
//	int y2 = min(min(scene_corners[1].y, scene_corners[3].y),
//				 min(obj_corners[1].y, obj_corners[3].y));

//	int x1,x2,y1,y2;
//
//	if(){
//		x1 = hor1;
//		x2 = hor2;
//		y1 = ver1;
//		y2 = ver2;
//	}
//	else{
//		x1 = ver1;
//		x2 = ver2;
//		y1 = hor1;
//		y2 = hor2;
//	}

	warpPerspective(i1,result,newHomography,Size(max(i2.cols-minCols,maxCols),max(i2.rows-minRows,maxRows)));
//	warpPerspective(i2,result,Mat::eye(Size(3,3),CV_64F),Size(i1.cols+i2.cols,i1.rows),INTER_LINEAR,BORDER_CONSTANT);

	cout << "No llega" <<endl;
	namedWindow("Warp",1);
	cout << "Jelou" << endl;
//	imshow("Warp", i1);
//	waitKey(0);
//	imshow("Warp", result);
//	waitKey(0);
	Mat half(result,Rect(-minCols,-minRows,i2.cols,i2.rows));
	cout << "half" << endl;

	i2.copyTo(half);

//	cout << "x1: " << x1 << endl;
//	cout << "x2: " << x2 << endl;
//	cout << "y1: " << y1 << endl;
//	cout << "y2: " << y2 << endl;

//	Mat final(result,Rect(x1,y1,x2-x1,y2-y1));
//	Mat final(result,Rect(0,48,553-0,262-48));
	cout << "copy" << endl;

	imshow("Warp", result);

	imwrite("Imagenes/result2.jpg",result);
	cout << "show" << endl;

	waitKey(0);

}

