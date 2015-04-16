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
	Mat d1, d2;
	vector< KeyPoint > kp1, kp2;
	vector< int > matches;
	Mat i_matches;

	SurfFeatureDetector detector(400);
	detector.detect(i1,kp1);
	detector.detect(i2,kp2);

	SurfDescriptorExtractor extractor;
	extractor.compute(i1,kp1,d1);
	extractor.compute(i2,kp2,d2);

	BruteForceMatcher< L2 < float > > matcher;
	matcher.add(d2);
	matcher.match(d1,matches);

	namedWindow("matches",1);
	drawMatches(i1,i2,kp1,kp2,matches,vector< char >(),i_matches);
	imshow("matches",i_matches);
	waitKey(0);
}
