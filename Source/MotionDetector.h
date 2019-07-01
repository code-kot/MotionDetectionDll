#pragma once
#include "API.h"
#include<chrono>
#include "cv.h"
#include <string>
#include "time_counter.h"

#define coef 6

using namespace std;
using namespace chrono;
using namespace cv;

class motion_detector
{
	const string display_window = "Motion detector";


	Ptr<BackgroundSubtractor> back_sub;

	static void bh_draw_color_label(Mat& src, const string& title, const Scalar& color, const int pos, const int size = 20); //function create text in window with image

	void refine_segments(const Mat& img, Mat& mask, Mat& dst, time_counter& t);	// function find contour from mask of image&find bounding rectangular from contour

	void show_images(Mat& img, Mat& mask);

	callback* callback_;
	

public:

	unsigned int frame_width_;
	unsigned int frame_height_;
	explicit motion_detector(callback* callback, unsigned int frame_width, unsigned int frame_height /* settings */);

	Mat view_mat; Mat temp; Mat mask, background, src1_resized;
	Mat* input_data;
	vector<Mat> src1;
	
	auto init();

	auto add_frame(Mat* input_data);

	auto reset();

	auto deinit();

	motion_detector();

	~motion_detector();	
};


