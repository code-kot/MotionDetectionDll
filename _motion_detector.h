#pragma once

#include <string>
#include<chrono>
#include "cv.h"
//#include <filesystem>

#include "time_counter.h"
#include "API.h"

//typedef unsigned long long H_instance;
//typedef void (*Callback)(int left, int top /* more params ?? */);

#define coef 6

using namespace std;
using namespace chrono;
using namespace cv;

class motion_detector
{
private:
	const string display_window = "Motion detector";
	Callback _callback{};
	Ptr<BackgroundSubtractor> back_sub;

	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos, const int size = 20); //function create text in window with image
	
	void refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t);	// function find contour from mask of image&find bounding rectangular from contour
	
	void show_images(Mat& img, Mat& mask);

	public:
		Mat view_mat; Mat temp; Mat mask, background, src1_resized;
		Mat* input_data;
		vector<Mat> src1;
	void init();
	
	auto add_frame(Mat* input_data);

	auto reset();

	auto deinit();

	auto _detector();
};