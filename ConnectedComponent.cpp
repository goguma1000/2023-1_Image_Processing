#include<iostream>
#include<opencv2/core.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<vector>

struct MouseParams {
	cv::Mat src;
	cv::Mat res;
};

void FindConnectedComponent(int event, int x, int y, int flag, void* userdata) {
	if (event == cv::EVENT_LBUTTONDOWN) {
		
		cv::Point seed = { x,y };
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_CROSS, cv::Size(9,9));

		MouseParams* mp = (MouseParams*)userdata;
		cv::Mat src = mp->src;
		cv::Mat res = mp->res;

		res.row(seed.y).col(seed.x) = 255;

		int convCount = 0;
		int preNonZeroCount = 0;
		int nowNonZeroCount = 0;
		cv::Mat temp;

		while(true){ 
			cv::dilate(res, res, kernel);
			cv::bitwise_and(src, res, res);
			convCount++;
			cv::compare(src, res, temp, cv::CMP_EQ);
			nowNonZeroCount = cv::countNonZero(temp);
			if (preNonZeroCount != nowNonZeroCount) {
				preNonZeroCount = nowNonZeroCount;
			}
			else break;
		}

		printf("Convergence count: %d\n", convCount);
		cv::imshow("res", res);
	}
}

void main() {
	cv::Mat src, res;
	src = cv::imread("connectedComponents.png",0);
	if (!src.empty()) {
		res = cv::Mat().zeros(src.size(), src.type());
	}

	MouseParams mp;
	mp.src = src;
	mp.res = res;

	cv::imshow("img", src);
	cv::setMouseCallback("img", FindConnectedComponent, &mp);
	cv::waitKey();
}