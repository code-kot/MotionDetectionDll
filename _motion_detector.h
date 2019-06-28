#pragma once
#include <string>
#include<chrono>
#include "cv.h"
#include <filesystem>

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
	Mat view_mat; Mat temp;
	const string display_window = "Motion detector";
	Callback _callback;
	Ptr<BackgroundSubtractor> back_sub;
	
public:
	
	auto add_frame(Mat* input_data)
	{
		auto& src1 = *input_data;
		//return init();
	}
	void init()
	{
		time_counter t; //create object T
		motion_detector m;
		back_sub = createBackgroundSubtractorMOG2(200, 90, true);
		Mat src1_resized;
		resize( &src1, src1_resized, Size(input.cols / coef, src1.rows / coef));
		Mat mask, background;
		back_sub->apply(src1_resized, mask);
		back_sub->getBackgroundImage(background);
	}
	void deinit()
	{
		
	}
	
	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos,
	                                const int size = 20) //function create text in window with image
	;

	void refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t)
	// function find contour from mask of image&find bounding rectangular from contour
	;

	void show_images(Mat& img, Mat& mask);

	void reset();
};