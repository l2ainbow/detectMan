// ShirtsDetection.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include <opencv2/core.hpp>    // coreモジュールのヘッダーをインクルード
#include <opencv2/highgui.hpp> // highguiモジュールのヘッダーをインクルード
#include <iostream>
#include <cmath>
#include <string>
#include <fstream>

using namespace cv;
using namespace std;

static void mask(Mat &gray, Mat &img) {
	Mat hsv = img.clone();
	cvCvtColor(&img, &hsv, CV_RGB2HSV_FULL);

	for (int i = 0; i < gray.cols; i++) {
		for (int j = 0; j < gray.rows; j++) {
			bool b = hsv.at<Vec3b>(j, i)[0] > 10;
			b = b && hsv.at<Vec3b>(j, i)[0] < 100;
			b = b && hsv.at<Vec3b>(j, i)[1] > 50;
			gray.at<Vec3d>(j, i) = b ? 255 : 0;
		}
	}

}

static void detectShirt(Mat &img, vector<Rect> &rects) {
	Mat gray = Mat::zeros(img.rows, img.cols, CV_8U);

	CvMemStorage *storage = cvCreateMemStorage(0);
	CvSeq *contours = 0;

	mask(gray, img);
	cvFindContours(&gray, storage, &contours, sizeof(CvContour));
	
	gray.release();

	for (int i = 0; i < contours->total; i++) {
		CvArr *approx = NULL;
		cvConvexHull2(&contours[i], approx);
		Rect rect = cvBoundingRect(approx);
		rects.push_back(cvBoundingRect(approx));
	}

	for (auto it = rects.end(); it != rects.begin(); it--) {
		auto x = *it;
		if ((x.width / x.height < 2)&(x.height / x.width > 0.5)) {
			rects.erase(it);
		}
	}

	cvReleaseMemStorage(&storage);

}

int main(int argc, const char* argv[])
{
	String filename = "IMG_5380/IMG_5380_0001.jpg";
	
	ifstream ifs(filename);
	if (!ifs.is_open()) {
		return -1;
	}


	Mat img = imread(filename, 1);
	//cout << img.cols << endl;

	//imshow("shirts", img);

	waitKey(0);

	vector<Rect> rects;
	detectShirt(img, rects);

	
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
		CvPoint p1 = cvPoint(rect.x, rect.y);
		CvPoint p2 = cvPoint(rect.x + rect.width, rect.y + rect.height);

		cvRectangle(&img, p1, p2, CV_RGB(255, 0, 0));
	}

	imshow("shirts", img);

	waitKey(0);

	destroyAllWindows();
	img.release();

	return 0;
}
