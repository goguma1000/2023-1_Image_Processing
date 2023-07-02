#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace cv;

void wienerfilter(Mat& input, Mat& output, Mat kernel,float k) {
	Mat input_dft, kernel_dft;
	dft(input, input_dft, DFT_COMPLEX_OUTPUT);
	dft(kernel, kernel_dft, DFT_COMPLEX_OUTPUT);
	
	std::vector<Mat> input_dftc, kernel_dftc;
	split(input_dft, input_dftc);
	split(kernel_dft, kernel_dftc);

	Mat denom, mags, H_inv;
	magnitude(kernel_dftc[0], kernel_dftc[1], mags);
	multiply(mags, mags, denom); //H^2
	denom += Scalar(k);
	denom = 1 / denom;
	Mat zz = Mat::zeros(denom.size(), denom.type());

	std::vector<Mat> dfc = { denom,zz };
	merge(dfc,H_inv);

	kernel_dftc[1] *= -1;
	merge(kernel_dftc, kernel_dft);
	
	mulSpectrums(input_dft, kernel_dft, output,DFT_ROWS);
	mulSpectrums(output, H_inv, output,DFT_ROWS);

	dft(output, output, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
}

void wienerfilterCaleach(Mat& input, Mat& output, Mat kernel, float k) {
	Mat input_dft, kernel_dft,rf;
	dft(input, input_dft, DFT_COMPLEX_OUTPUT);
	dft(kernel, kernel_dft, DFT_COMPLEX_OUTPUT);
	rf.create(input_dft.size(), input_dft.type());

	for (int y = 0; y < rf.rows; y++){
		for (int x = 0; x < rf.cols; x++) {
			float a = kernel_dft.at<Vec2f>(y, x)[0];
			float b = kernel_dft.at<Vec2f>(y, x)[1];
			float c = input_dft.at<Vec2f>(y, x)[0];
			float d = input_dft.at<Vec2f>(y, x)[1];

			Vec2f value = Vec2f((a * c + b * d), (a * d - b * c)) / (a * a + b * b + k);
			rf.at<Vec2f>(y, x) = value;
		}
	}

	dft(rf, output, DFT_INVERSE | DFT_SCALE | DFT_REAL_OUTPUT);
}

void main() {
	Mat kernel, img1, img2, img3, img4;
	kernel = imread("Wiener_Kernel.png",0);
	kernel.convertTo(kernel, CV_32FC1, 1/255.f);
	kernel /= sum(kernel)[0];
	img1 = imread("Wiener_Input1.png",0);
	img1.convertTo(img1, CV_32FC1,1/255.f);
	img2 = imread("Wiener_Input2.png",0);
	img2.convertTo(img2, CV_32FC1, 1 / 255.f);

	img3 = imread("Wiener_Input1.png",0);
	img3.convertTo(img3, CV_32FC1,1/255.f);
	img4 = imread("Wiener_Input2.png",0);
	img4.convertTo(img4, CV_32FC1, 1 / 255.f);

	imshow("kernel", kernel);
	imshow("img1", img1);
	imshow("img2", img2);
	Mat img1_out,img2_out, img3_out, img4_out;
	
	wienerfilter(img1, img1_out, kernel, 0.00012);
	wienerfilter(img2, img2_out, kernel, 0.00225);
	wienerfilterCaleach(img3, img3_out, kernel, 0.00012);
	wienerfilterCaleach(img4, img4_out, kernel, 0.00225);
	
	imshow("img1_out", img1_out);
	imshow("img2_out", img2_out);
	imshow("img3_out", img3_out);
	imshow("img4_out", img4_out);
	waitKey();
}