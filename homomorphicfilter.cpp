#include <iostream>
#include <opencv2/core.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
using namespace cv;

Mat HomomorphicFilter(Mat& inarr, Mat& outarr, float gammaH, float gammaL, float d0) {
	Mat H = Mat::zeros(inarr.size(), CV_32FC1);
	inarr.convertTo(inarr, CV_32FC1, 1 / 255.f);
	inarr += Scalar::all(1);
	log(inarr, inarr);
	dft(inarr, inarr, DFT_COMPLEX_OUTPUT);
	for (int y = 0; y < inarr.rows; y++) {
		for (int x = 0; x < inarr.cols; x++) {
			float dx, dy;
			dx = x > inarr.cols / 2 ? inarr.cols - x : x;
			dy = y > inarr.rows / 2 ? inarr.rows - y : y;
			float d2 = dx * dx + dy * dy;
			H.at<float>(y, x) = (gammaH - gammaL) * (1 - exp(-(d2 / (d0 * d0)))) + gammaL;
		}
	}
	Mat zz = Mat::zeros(inarr.size(), CV_32FC1);
	std::vector<Mat> temp = { H, zz };
	merge(temp, H);
	mulSpectrums(inarr, H, outarr, DFT_ROWS);
	dft(outarr, outarr, DFT_SCALE | DFT_INVERSE | DFT_REAL_OUTPUT);
	exp(outarr, outarr);
	outarr -= Scalar::all(1);
	return outarr;
}

int main() {
	Mat img, logimg, dftimg, res;
	img = imread("homomorphicInput.jpeg",0);
	imshow("img", img);
	HomomorphicFilter(img, res, 2, 0.89, 25);
	imshow("res", res);
	waitKey();
}