// ShirtsDetection.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
//#include "stdafx.h"

#include <opencv2/core.hpp>    // coreモジュールのヘッダーをインクルード
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>
#include "cpp.hpp"
using namespace cv;
using namespace std;

static void mask(Mat &gray, Mat &img) {
	IplImage iplImage = img;
	IplImage *hsv = cvCreateImage(cvGetSize(&iplImage), IPL_DEPTH_8U, 3);
	cvCvtColor(&iplImage, hsv, CV_RGB2HSV_FULL);
	for (int y = 0; y < hsv->height; y++) {
		for (int x = 0; x < hsv->width; x++) {
			bool b = hsv->imageData[hsv->widthStep*y + x * 3] > 10;
			b = b && hsv->imageData[hsv->widthStep*y + x * 3] < 100;
			b = b && hsv->imageData[hsv->widthStep*y + x * 3 + 1] > 50;
			gray.at<uchar>(y, x) = b ? 255 : 0;
		}
	}

	cvReleaseImage(&hsv);
}

static void detectShirt(Mat &img, vector<Rect> &rects) {
	Mat gray = Mat::zeros(img.rows, img.cols, CV_8U);

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;

	mask(gray, img);
	IplImage iplImage = gray;
	cvFindContours(&iplImage, storage, &contours, sizeof(CvContour));

	gray.release();

	for (; contours != 0; contours = contours->h_next) {
		Rect rect = cvBoundingRect(contours, 1);
		auto x = rect;
		if (!(x.width / x.height < 2) || !(x.height / x.width > 0.5)) {
			rects.push_back(rect);
		}
	}

	cvReleaseMemStorage(&storage);
}

double calcDist(double r, double rmax) {
	double f = rmax / (43.2 / 2 / 32);
	double theta = r / f;

	//height = 1m is assumed
	return tan(theta);
}

string detectMan(Mat &img, double oldD, double oldTheta) {
	IplImage iplImage = img;
	IplImage *resized = cvCreateImage(cvSize(iplImage.width / 5, iplImage.height / 5), IPL_DEPTH_8U, 3);
	cvResize(&iplImage, resized);

	img.release();
	img = cvarrToMat(resized);

	vector<Rect> rects;
	detectShirt(img, rects);

	double w = img.cols;
	double h = img.rows;

	double r, theta;
	if (rects.size() > 0) {
		Rect rect;
		double max = 0;
		for (auto it = rects.begin(); it != rects.end(); it++) {
			double area = (*it).width * (*it).height;
			if (area > max) {
				max = area;
				rect = *it;
			}
		}
		double x = rect.x + rect.width / 2 - w / 2;
		double y = rect.y + rect.height / 2 - h / 2;

		r = sqrt(pow(x, 2) + pow(y, 2));
		theta = atan2(x, y);
	}

	cvReleaseImage(&resized);
	img.release();

	int d = calcDist(r, w);

	ostringstream ss, ss1;
	ss << d;
	ss1 << theta;

	string str = ss.str() + "," + ss1.str();

	return str;
}
