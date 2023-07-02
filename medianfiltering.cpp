#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>
using namespace cv;
using namespace std;

void merge(vector<uchar>& arr, vector<uchar>& carr, int start, int mid, int end) {
	int i = start;
	int  j = mid;
	for (int k = start; k < end; k++) {
		if (i < mid && (j >= end || arr[i] <= arr[j])) {
			carr[k] = arr[i];
			i++;
		}
		else {
			carr[k] = arr[j];
			j++;
		}
	}
}
void copyarr(vector<uchar>& arr, vector<uchar>& carr) {
	for (int i = 0; i < arr.size(); i++) {
		carr[i] = arr[i];
	}
}
void splitmerge(vector<uchar>& arr, vector<uchar>& carr, int start, int end) {

	if (end - start < 2) return;

	int mid = (start + end) / 2;
	splitmerge(carr, arr, start, mid);
	splitmerge(carr, arr, mid, end);

	merge(arr, carr, start, mid, end);
}
void mergesort(vector<uchar>& arr) {
	vector<uchar> carr(arr.size());
	copyarr(arr, carr);
	int start = 0;
	int end = arr.size();
	splitmerge(carr, arr, start, end);
}



void MedianFilter(cv::Mat& img, cv::Mat& out, int Filtersize) {
	if (Filtersize % 2 == 0) Filtersize += 1;
	vector<uchar> filter;
	for (int i = 0; i < img.rows; i++) {
		for (int j = 0; j < img.cols; j++) {
			//주변 픽셀 값 읽기
			for (int di = (1 - Filtersize) / 2; di <= (Filtersize - 1) / 2; di++) {
				for (int dj = (1 - Filtersize) / 2; dj <= (Filtersize - 1) / 2; dj++) {
					int xx = j + dj;
					int yy = i + di;
					//use reflect101 padding
					if (xx < 0) xx = -xx;
					else if (xx > img.cols - 1)  xx = 2 * img.cols - 2 - xx;
					if (yy < 0) yy = -yy;
					else if (yy > img.rows - 1) yy = 2 * img.rows - 2 - yy;
					filter.push_back(img.at<uchar>(yy,xx));
				}
			}
			mergesort(filter);
			printf("%d ", out.at<uchar>(i, j) = filter[(filter.size() - 1) / 2]);
			if (filter.size() == 0) {
				printf("error\n");
			}
			out.at<uchar>(i, j) = filter[(filter.size() -1)/2];
			filter.clear();
		}
	}
}

int main() {
	Mat img = imread("img.jpg",0);
	imshow("img", img);
	MedianFilter(img, out, 5);
	Mat out(img.size(),img.type());
	imshow("img2", out);
	medianBlur(img, img, 5);
	imshow("img3", img);
	waitKey();
}
