#include "opencv2/highgui/highgui.hpp"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <iostream>
#include <math.h>
using namespace std;
using namespace cv;
#define pi 3.141592653
int horizon(Vec4i l1, Vec4i l2)
{
	double m1, m2, c1, c2;

	m1=((double)l1[3]-l1[1])/((double)l1[2]-l1[0]);
	c1=(double)l1[3]-m1*l1[2];

	m2=((double)l2[3]-l2[1])/((double)l2[2]-l2[0]);
	c2=(double)l2[3]-m2*l2[2];

	double yi, xi;
	if(m1!=0||m2!=0)
	
	{
		xi=(c1-c2)/(m2-m1);
		yi=m2*xi+c2;
	}
	else
	{
		return 10000;
	}   


	//if lines are parallel and on road surface but orientation not correct
	if(int(xi)<-200 || int(xi)>1200)
		return 10000;

	return (int)yi;
}

int main()
{
	Mat img=imread("test.jpg", 1);  

	int low_threshold=50;
	int high_threshold=100;
	namedWindow("hough", WINDOW_AUTOSIZE);
	namedWindow("canny", WINDOW_AUTOSIZE);
	namedWindow("image", WINDOW_AUTOSIZE);

	int minLinLength=50;
	int maxLineGap=10;
	//createTrackbar("low_threshold", "canny", &low_threshold, 255 );
	//createTrackbar("high_threshold", "canny", &high_threshold, 255);
	//createTrackbar("minLinLength", "hough", &minLinLength, 500 );
	//createTrackbar("maxLineGap", "hough", &maxLineGap, 50);
	
	while(1){
		Mat edge(img.rows, img.cols, CV_8UC1, Scalar(0));
		Mat hough(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
		Mat dst(img.rows, img.cols, CV_8UC3, Scalar(0,0,0));
		Canny(img, edge, low_threshold, high_threshold);
		vector<Vec4i> lines;
		HoughLinesP(edge, lines, 1, CV_PI/180, 50, minLinLength, maxLineGap );
		for( size_t i = 0; i < lines.size(); i++ )
		{
			Vec4i l = lines[i];
			float angle;
			angle=atan2(((float)l[3] - (float)l[1]), (float)l[2] - (float)l[0]);
			if((fabs(angle)*180/CV_PI)>20){
			line( hough, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255),0.5, CV_AA);}
			
		}
		imshow("image", img);
		imshow("canny", edge);
		imshow("hough", hough);
		
		if (waitKey(3)){
			break;
		}
	}
	return 0;
}